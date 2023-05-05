#include "main.h"

static	uint8_t	***allocate_cave_map(unsigned int size)
{
	uint8_t	***new;

	if (!(new = (uint8_t***)malloc(sizeof(uint8_t**) * CHUNK_SIZE)))
		return (NULL);

	for (unsigned int i = 0; i < CHUNK_SIZE; i++)
	{
		if (!(new[i] = (uint8_t**)malloc(sizeof(uint8_t*) * size)))
			return (NULL);

		for (unsigned int j = 0; j < size; j++)
		{
			if (!(new[i][j] = (uint8_t*)malloc(sizeof(uint8_t) * CHUNK_SIZE)))
				return (NULL);
			memset(new[i][j], 0, sizeof(uint8_t) * CHUNK_SIZE);
		}

	}
	return (new);
}

float randbycoords(int x, int y) {
	x += INT_MAX / 2;
	y += INT_MAX / 2;
  /* mix around the bits in x: */
  x = x * 3266489917 + 374761393;
  x = (x << 17) | (x >> 15);

  /* mix around the bits in y and mix those into x: */
  x += y * 3266489917;

  /* Give x a good stir: */
  x *= 668265263;
  x ^= x >> 15;
  x *= 2246822519;
  x ^= x >> 13;
  x *= 3266489917;
  x ^= x >> 16;

  /* trim the result and scale it to a float in [0,1): */
  return (x & 0x00ffffff) * (1.0f / 0x1000000);
}

unsigned char	init_worley_points(t_env *env, t_chunk *chunk, unsigned int size)
{
	(void)env;

	int	seed = (int)(*map_seed() * randbycoords(chunk->x_start, chunk->z_start));

	srand(seed);

	for (unsigned int i = 0; i < NB_WORLEY_POINTS; i++)
	{
		chunk->wpoints[i].x = rand() % CHUNK_SIZE;
		chunk->wpoints[i].y = rand() % size;
		chunk->wpoints[i].z = rand() % CHUNK_SIZE;

		chunk->wpoints[i].x += chunk->x_start;
		//chunk->wpoints[i].y += CAVE_DEPTH;
		chunk->wpoints[i].z += chunk->z_start;
	}
	return (ERR_NONE);
}

static int	dist(t_3dpoint a, t_3dpoint b)
{
	int		dx = b.x - a.x;
	int		dy = b.y - a.y;
	int		dz = b.z - a.z;

	return (abs(dx * dx) + abs(dy * dy) + abs(dz * dz));
}
/*
void	sort_distances(float arr[NB_WORLEY_POINTS], int n)
{
    int i, key, j;
    for (i = 1; i < n; i++)
    {
        key = arr[i];
        j = i - 1;
 
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}*/

static void get_three_lowests(float distances[NB_WORLEY_POINTS * 9], float *first, float *third)
{
	float	tmp;
	float	a, b, c, d;

	a = 1000000.0f;
	b = 1000000.0f;
	c = 1000000.0f;

	for (int i = 0; i < NB_WORLEY_POINTS * 9; i++)
	{
		d = distances[i];

		//printf("res.x : %p | res.y : %p |res.z : %p |d : %p |distances : %p\n", &res.x, &res.y, &res.z, &d, &distances);
		if (d < c)
			c = d;

		if (c < b)
		{
			tmp = b;
			b = c;
			c = tmp;
		}

		if (b < a)
		{
			tmp = a;
			a = b;
			b = tmp;
		}
	}

	*first = a;
	*third = c;
}

static unsigned char	get_points(t_env *env, t_chunk *chunk, t_3dpoint points[NB_WORLEY_POINTS * 9])
{
	t_chunk		*tmp;
	int			current_x, current_z;
	int			index = 0;

	(void)chunk;

	for (unsigned int i = 0; i < NB_WORLEY_POINTS * 9; i++)
		points[i] = (t_3dpoint){UCHAR_MAX, UCHAR_MAX, UCHAR_MAX};

//	printf("--------\n");
	for (int x = 0; x < SQUARE_SIZE; x++)
		for (int z = 0; z < SQUARE_SIZE; z++)
		{
			if (abs(env->model.chunks[x][z].x_start - chunk->x_start) <= CHUNK_SIZE
				&& abs(env->model.chunks[x][z].z_start - chunk->z_start) <= CHUNK_SIZE)
			{
				for (int i = 0; i < NB_WORLEY_POINTS; i++)
				{
					points[index++] = env->model.chunks[x][z].wpoints[i];
//					printf("%d %d %d | %d cells\n", env->model.chunks[x][z].wpoints[i].x, env->model.chunks[x][z].wpoints[i].y, env->model.chunks[x][z].wpoints[i].z, points->nb_cells);
					//if (dynarray_push(points, &env->model.chunks[x][z].wpoints[i], false))
					//	return (ERR_MALLOC_FAILED);
				}
			}
		}
//	printf("--------\n");
//	fflush(stdout);
	return (ERR_NONE);
}

static unsigned char	worley_noise(t_env *env, t_chunk *chunk, t_3dpoint points[NB_WORLEY_POINTS * 9], float distances[NB_WORLEY_POINTS * 9], t_3dpoint current, uint8_t *res)
{
	t_3dpoint	*wpoint;
	float		min_dist = INFINITY;
	float		distance;
	float		a, c;

	(void)env;
	current.x += chunk->x_start;
	current.z += chunk->z_start;
	for (int i = 0; i < NB_WORLEY_POINTS * 9; i++)
	{
		distances[i] = dist(points[i], current);
		min_dist = fmin(distances[i], min_dist);
	}

	get_three_lowests(distances, &a, &c);

	if (a / c > WORLEY_THRESHOLD)
		*res = BT_STONE;

	memset(distances, 0, sizeof(float) * NB_WORLEY_POINTS * 9);

	return (ERR_NONE);
}

unsigned char			generate_cave_map(t_env *env, t_chunk *chunk, unsigned int size)
{
	float			distances[NB_WORLEY_POINTS * 9];
	t_3dpoint		points[NB_WORLEY_POINTS * 9];
	unsigned char	code;

	if (!(chunk->cave_map = allocate_cave_map(size))
		|| get_points(env, chunk, points))
		return (ERR_MALLOC_FAILED);

	for (unsigned int x = 0; x < CHUNK_SIZE; x++)
	{
		for (unsigned int y = 0; y < size; y++)
		{
			for (unsigned int z = 0; z < CHUNK_SIZE; z++)
			{
				//printf("%d/%d  %d/%d  %d/%d\n", x, CHUNK_SIZE, y, size, z, CHUNK_SIZE);
				//printf("%p\n", &chunk->cave_map[x][y][z]);
				if ((code = worley_noise(env, chunk, points, distances, (t_3dpoint){x, y, z}, &chunk->cave_map[x][y][z])))
					return (code);
			}
		}
	}

	/*
	for (unsigned int x = 0; x < size; x++)
	{
		for (unsigned int y = 0; y < size; y++)
		{
			for (unsigned int z = 0; z < size; z++)
				if (chunk->cave_map[x][y][z] == 0)
					printf(". ");
				else
					printf("0 ");
			printf("\n");
		}
		printf("\n");
		usleep(100000);
	}

	fflush(stdout);*/

	return (ERR_NONE);
}
