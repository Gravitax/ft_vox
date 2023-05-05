#include "main.h"


/*
 * This function is designed to display error messages depending
 * on the value return by the error chain, if anyone.
 */

void			error_handler(t_env *env, unsigned char code)
{
	const char	*usage = "./ft_vox\n"; // Usage message
	const char	*msg;

	if (code >= ERR_MAX) // If code does not correspond to any existing code
		return ;

	msg = &errors[code].msg[0]; // Get the current error message

	// Display it
	if (write(2, "ft_vox : ", 7) == 0
		|| write(2, "error : ", 8) == 0
		|| write(2, msg, ft_strlen(msg)) == 0
		|| write(2, "\n", 1) == 0)
		return ;

	// Display usage if needed
	if (errors[code].usage && write(2, usage, ft_strlen(usage)))
		return ;

	// Free environment content before the main returns
	free_env(env);
}
