# include "main.h"

const uint8_t	third = (BTB_BEACH - BTB_CAVE) / 3;
const uint8_t	water_level = BTB_BEACH - third;

static bool		is_water(uint8_t y)
{
	return (y < water_level);
}

static unsigned char	pour_water(t_chunk *chunk, unsigned int x, unsigned int y, unsigned int z)
{
	vec3			top_plane[6];
	float			xx, yy, zz;
	vec3			a, b, c, d;
	unsigned char	code;

	xx = x;
	yy = y;
	zz = z;

	// Cube's top plane ABCD points
	a = (vec3){xx, yy, (z + 1)};
	b = (vec3){(x + 1), yy, (z + 1)};
	c = (vec3){xx, yy, zz};
	d = (vec3){(x + 1), yy, zz};

	top_plane[0] = c;
	top_plane[1] = a;
	top_plane[2] = b;
	top_plane[3] = c;
	top_plane[4] = b;
	top_plane[5] = d;

	return (push_plane(chunk, top_plane, N_UP, y, 1.0f, false, true));
}

unsigned char			generate_water(t_chunk *chunk)
{
	unsigned char	code;
	unsigned int	y;

	for (unsigned int x = 0; x < CHUNK_SIZE; x++)
		for (unsigned int z = 0; z < CHUNK_SIZE; z++)
		{
			y = chunk->surface_hmap[x][z];
			if (is_water(y) && (code = pour_water(chunk, chunk->x_start + x, water_level - 1, chunk->z_start + z)))
				return (code);
		}
	return (ERR_NONE);
}
