#include "main.h"

int		*map_seed(void)
{
	static int seed = -1;

	if (seed == -1)
		seed = rand();

	return (&seed);
}

