# include "main.h"

static unsigned char	read_seed(int argc, char **argv)
{
	long long int	seed;
	if (argc > 2)
		return (ERR_INVALID_ARGC);

	for (unsigned int i = 0; argv[1][i]; i++)
		if (i > 10 || ft_isdigit(argv[1][i]) == false)
			return (ERR_INVALID_SEED);

	seed = ft_atoi(argv[1]);

	*map_seed() = seed;

	return (ERR_NONE);
}

static unsigned char	init_map(t_env *env)
{
	unsigned char	code;
	int				x_start, z_start;

	env->model.square_x = 512;
	env->model.square_z = 512;
	for (unsigned int x = 0; x < SQUARE_SIZE; x++)
		for (unsigned int z = 0; z < SQUARE_SIZE; z++)
		{
			x_start = (env->model.square_x + x) * CHUNK_SIZE;
			z_start = (env->model.square_z + z) * CHUNK_SIZE;
			env->model.chunks[x][z].x_start = x_start;
			env->model.chunks[x][z].z_start = z_start;
			if ((code = init_worley_points(env, &env->model.chunks[x][z], CAVE_DEPTH)))
				return (code);
		}

	for (unsigned int x = 0; x < SQUARE_SIZE; x++)
	{
		for (unsigned int z = 0; z < SQUARE_SIZE; z++)
		{
			x_start = (env->model.square_x + x) * CHUNK_SIZE;
			z_start = (env->model.square_z + z) * CHUNK_SIZE;

			if ((code = gen_chunk(env, &env->model.chunks[x][z], x_start, z_start, true)))
				return (code);

			env->model.chunks[x][z].chunk_x = x;
			env->model.chunks[x][z].chunk_z = z;
		}
	}

	for (unsigned int x = 0; x < SQUARE_SIZE; x++)
		for (unsigned int z = 0; z < SQUARE_SIZE; z++)
			if ((code = fix_chunk_borders(env, x, z)) != ERR_NONE
				|| (code = generate_water(&env->model.chunks[x][z])))
				return (code);

	printf("%zu bytes per vertex | %.2f Mo on heap\n", sizeof(t_stride), *stride_bytesize() / 1000000.0f);
	return (ERR_NONE);
}

unsigned char			init_world(t_env *env, int argc, char **argv)
{
	unsigned char	code;

	if ((argc > 1 && (code = read_seed(argc, argv)) != ERR_NONE)
		|| (code = init_map(env)) != ERR_NONE)
		return (code);

	printf("seed : %d\n", *map_seed());

	if ((code = model(env)) != ERR_NONE)
		return (code);

	camera(env); // camera after load settings cause we need ww and wh
	// camera before light because we need campos
	light(env);

	return (ERR_NONE);
}
