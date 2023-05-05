# include "main.h"

static bool				is_chunk_on_border(t_env *env, int x_start, int z_start)
{
	const int	last = SQUARE_SIZE - 1;

	return  (env->model.chunks[0][0].x_start == x_start
			|| env->model.chunks[0][0].z_start == z_start
			|| env->model.chunks[last][last].x_start == x_start
			|| env->model.chunks[last][last].z_start == z_start);
}

// static unsigned char	generate_cave_vertexs(t_chunk *chunk, int x_start, int z_start)
// {
// 	unsigned char	code;

// 	for (unsigned int x = 0; x < CHUNK_SIZE; x++)
// 		for (unsigned int z = 0; z < CHUNK_SIZE; z++)
// 		{
// 			if ((code = generate_cave_column(chunk, x, z, x_start, z_start)))
// 				return (code);
// 		}
// 	return (generate_cave_floor(chunk));
// }

static unsigned char	generate_surface_vertexs(t_chunk *chunk, int x_start, int z_start)
{
	unsigned int	y;
	vec3			top_plane[6];
	unsigned char	code;

	for (unsigned int x = 0; x < CHUNK_SIZE; x++)
		for (unsigned int z = 0; z < CHUNK_SIZE; z++)
		{
			y = chunk->surface_hmap[x][z];

			if ((code = generate_top_plane(chunk, x + x_start, y, z + z_start, top_plane)) != ERR_NONE
				|| (code = generate_side_plane(chunk, chunk->surface_hmap, x, y, z, CHUNK_SIZE, top_plane)) != ERR_NONE)
				return (code);
		}
	return (ERR_NONE);
}

static unsigned char	generate_vertexs(t_env *env, t_chunk *chunk, int x_start, int z_start)
{
	unsigned char	code;

	if ((code = generate_surface_vertexs(chunk, x_start, z_start)))
		return (code);

	if (!is_chunk_on_border(env, x_start, z_start) != ERR_NONE)
		// && (code = generate_cave_vertexs(chunk, x_start, z_start)) != ERR_NONE)
		return (code);

	return (ERR_NONE);
}

// unsigned char	generate_last_caves(t_env *env)
// {
// 	t_chunk			*chunk;
// 	unsigned char	code;

// 	for (unsigned int x = 1; x < SQUARE_SIZE - 1; x++)
// 		for (unsigned int z = 1; z < SQUARE_SIZE - 1; z++)
// 		{
// 			chunk = &env->model.chunks[x][z];

// 			if (chunk->cave_map == NULL)
// 				if ((code = generate_cave_map(env, chunk, CAVE_DEPTH)) != ERR_NONE)
// 					return (code);

// 			if ((code = generate_cave_vertexs(chunk, chunk->x_start, chunk->z_start)) != ERR_NONE)
// 				return (code);
// 		}
// 	return (ERR_NONE);
// }

static unsigned char	generate_chunk_content(t_env *env, t_chunk *chunk, int x_start, int z_start, bool stride)
{
	unsigned char	code;
	t_biome_params	surface_params = {0.02f, 6.0f, 0.0f, 0.0f};
	t_biome_params	sub_params = {0.05f, 6.0f, 0.0f, 40.0f};

	chunk->x_start = x_start;
	chunk->z_start = z_start;

	(void)env;
	if (!chunk->surface_hmap || !chunk->cave_map)
	{
		// Load parameters for the current chunk
		//printf("frequency : %f | depth : %f\n", params.frequency, params.depth);
		// Generate height maps for surface and cave
		// Topography type should be a parameter which would affect perlin noise generation
		if (!(chunk->surface_hmap = generate_height_map(surface_params, x_start, z_start, CHUNK_SIZE, 40)))
			return (ERR_MALLOC_FAILED);

		if (!is_chunk_on_border(env, x_start, z_start) && (code = generate_cave_map(env, chunk, CAVE_DEPTH)))
			return (code);
		else if ((code = init_worley_points(env, chunk, CAVE_DEPTH)))
			return (code);
	}

	if (stride)
	{
		if (dynarray_init(&chunk->stride, sizeof(t_stride), CHUNK_SIZE * CHUNK_SIZE * 6 * 2))
			return (ERR_MALLOC_FAILED);

		if ((code = generate_vertexs(env, chunk, x_start, z_start)) != ERR_NONE)
			return (code);
	}

	return (ERR_NONE);
}

// This function will be used to generate new chunks of terrain with different noise algorithms

size_t	*stride_bytesize(void)
{
	static size_t	bytesize = 0;

	return (&bytesize);
}

unsigned char			gen_chunk(t_env *env, t_chunk *chunk, int x_start, int z_start, bool stride)
{
	unsigned char	code;

	if ((code = generate_chunk_content(env, chunk, x_start, z_start, stride)) != ERR_NONE)
		return (code);

	*stride_bytesize() += sizeof(t_stride) * chunk->stride.nb_cells;
	return (ERR_NONE);
}
