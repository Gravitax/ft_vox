#include "main.h"

static	uint8_t	**allocate_height_map(unsigned int size)
{
	uint8_t	**new;

	if (!(new = (uint8_t**)malloc(sizeof(uint8_t*) * size)))
		return (NULL);

	for (unsigned int i = 0; i < size; i++)
	{
		if (!(new[i] = (uint8_t*)malloc(sizeof(uint8_t) * size)))
			return (NULL);
		memset(new[i], 0, sizeof(uint8_t) * size);
	}

	return (new);
}

uint8_t	**generate_height_map(t_biome_params params, int x_start, int z_start, unsigned int size, unsigned char offset)
{
	uint8_t	**hmap;
	uint8_t	height;
	unsigned int	xbound, zbound;

	if (!(hmap = allocate_height_map(size)))
		return (NULL);

	xbound = x_start + size;
	zbound = z_start + size;

	for (unsigned int x = x_start; x < xbound; x++)
		for (unsigned int z = z_start; z < zbound; z++)
		{
			height = (uint8_t)(perlin2d_a((float)x, (float)z, params.frequency, params.depth) * size);
			hmap[x - x_start][z - z_start] = height + offset;
		}
	return (hmap);
}
