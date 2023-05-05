#include "main.h"


static unsigned char	ft_vox(t_env *env, int argc, char **argv)
{
	unsigned char	code;

	// Initialization
	if ((code = init(env, argc, argv)) != ERR_NONE)
		return (code);

	// Main loop launcher
	return (display_loop(env));
}

int						main(int argc, char **argv)
{
	t_env			env;
	unsigned char	code;

	memset(&env, 0, sizeof(t_env));
	if ((code = ft_vox(&env, argc, argv)) != ERR_NONE)
	{
		error_handler(&env, code);
		return (code); // errors defined in error.h
	}

	free_env(&env);

	return (ERR_NONE);
}
