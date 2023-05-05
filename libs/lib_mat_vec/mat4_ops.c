#include "lib_mat_vec.h"


void		mat4_multiply(mat4 a, mat4 b)
{
	unsigned int	i, j;
	mat4			res;

	for (j = 0; j < 4; j++)
		for (i = 0; i < 4; i++)
		{
			res[j * 4 + i] = a[j * 4 + 0] * b[0 * 4 + i]
				+ a[j * 4 + 1] * b[1 * 4 + i]
				+ a[j * 4 + 2] * b[2 * 4 + i]
				+ a[j * 4 + 3] * b[3 * 4 + i];
		}
	memcpy(a, res, sizeof(mat4));
}

vec3		mat4_x_vec3(mat4 m, vec3 v)
{
	vec3	out;

	out.x = m[0] * v.x + m[1] * v.y + m[2] * v.z + m[3];
	out.y = m[4] * v.x + m[5] * v.y + m[6] * v.z + m[7];
	out.z = m[8] * v.x + m[9] * v.y + m[10] * v.z + m[11];
	return (out);
}
