#include "main.h"

static const vec3	n[6] =	{
								[N_NORTH] = (vec3){0, 0, 1},
								[N_SOUTH] = (vec3){0, 0, -1},
								[N_WEST] = (vec3){-1, 0, 0},
								[N_EAST] = (vec3){1, 0, 0},
								[N_UP] = (vec3){0, 1, 0},
								[N_DOWN] = (vec3){0, -1, 0}
								};

unsigned char			generate_cave_floor(t_chunk *chunk)
{
	t_stride	vertex;
	vec3		a, b, c, d;
	vec3		plane[6];

	a = (vec3){chunk->x_start, CAVE_DEPTH, chunk->z_start + CHUNK_SIZE};
	b = (vec3){chunk->x_start + CHUNK_SIZE, CAVE_DEPTH, chunk->z_start + CHUNK_SIZE};
	c = (vec3){chunk->x_start, CAVE_DEPTH, chunk->z_start};
	d = (vec3){chunk->x_start + CHUNK_SIZE, CAVE_DEPTH, chunk->z_start};

	plane[0] = c;
	plane[1] = a;
	plane[2] = b;
	plane[3] = c;
	plane[4] = b;
	plane[5] = d;

	for (unsigned int i = 0; i < 6; i++)
	{
		memset(&vertex, 0, sizeof(t_stride));

		// Constuction of the vertex
		vertex = (t_stride){(short)plane[i].x, 
							(short)plane[i].y,
							(short)plane[i].z,
							(uint8_t)i,
							(uint8_t)((int)CHUNK_SIZE - 1),
							(uint8_t)N_UP,
							(uint8_t)BT_STONE,
							(uint8_t)1};

		// Insertion of the vertex in the stride
		if (dynarray_push(&chunk->stride, &vertex, false))
			return (ERR_MALLOC_FAILED);
	}
	return (ERR_NONE);
}

static unsigned int		check_neighbours(t_chunk *chunk, unsigned char neighbours[N_MAX],
																					unsigned int x,
																					unsigned int y,
																					unsigned int z)
{
	// neighbours coordinates
	unsigned int	index = 0;
	unsigned int	tx, ty, tz;

	memset(neighbours, 0, sizeof(unsigned char) * 6);
	for (unsigned int i = 0; i < N_MAX; i++)
	{
		tx = x + n[i].x;
		ty = y + n[i].y;
		tz = z + n[i].z;

		if (tx >= CHUNK_SIZE
			|| ty >= CAVE_DEPTH
			|| tz >= CHUNK_SIZE)
			continue;

		if (chunk->cave_map[tx][ty][tz] == BT_STONE)
			neighbours[index++] = (unsigned char)i;
	}

	return (index);
}

unsigned char			generate_cave_column(t_chunk *chunk, unsigned int x, unsigned int z,
												unsigned int x_start, unsigned int z_start)
{
	unsigned			plain_neighbours = 0;
	unsigned char		neighbours[N_MAX];
	unsigned char		code;
	const unsigned int	offset = 32;
	vec3				a, b;
	vec3				top_plane[6];
	unsigned int		heights[N_MAX] = {0, 0, 0, 0}; // greedy meshing
	unsigned int		y_pos[N_MAX] = {0, 0, 0, 0}; // greedy meshing

	const vec3			a_points[N_MAX] = {
						[N_WEST] = (vec3){0, 1, 0},
						[N_EAST] = (vec3){1, 1, 0},
						[N_NORTH] = (vec3){0, 1, 1},
						[N_SOUTH] = (vec3){0, 1, 0}
	};

	const vec3			b_points[N_MAX] = {
						[N_WEST] = (vec3){0, 0, 1},
						[N_EAST] = (vec3){0, 0, 1},
						[N_NORTH] = (vec3){1, 0, 0},
						[N_SOUTH] = (vec3){1, 0, 0}
	};

	for (unsigned int y = offset; y < CAVE_DEPTH + offset; y++)
	{
		if (chunk->cave_map[x][y - offset][z] != BT_STONE)
		{
			plain_neighbours = check_neighbours(chunk, neighbours, x, y - CAVE_DEPTH, z);
			for (unsigned int i = 0; i < plain_neighbours; i++)
			{
				if (neighbours[i] == N_UP)
				{
					if ((code = generate_top_plane(chunk, x_start + x, y + 1, z_start + z, top_plane)))
						return (code);
				}

				else if (neighbours[i] == N_DOWN)
				{
					if ((code = generate_top_plane(chunk, x_start + x, y, z_start + z, top_plane)))
						return (code);
				}

				else
				{
					a = vec_add((vec3){x_start + x, y, z_start + z}, a_points[neighbours[i]]);
					b = vec_add(a, b_points[neighbours[i]]);
					if ((code = generate_fall(chunk, a, b, neighbours[i], y + 1, 1.0f)))
						return (code);
				}
			}
		}
	}
	return (ERR_NONE);
}
