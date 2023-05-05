#include "lib_mat_vec.h"


/*
	1 0 0 0
	0 1 0 0
	0 0 1 0
	0 0 0 1
*/
void		mat4_identity(mat4 m)
{
	memset(m, 0, sizeof(mat4));
	m[0] = 1;
	m[5] = 1;
	m[10] = 1;
	m[15] = 1;
}

/*
	e 0 0 0
	0 e 0 0
	0 0 e 0
	e e e e
*/
void		mat4_bias(mat4 m, float e)
{
	m[0] = e;
	m[1] = 0;
	m[2] = 0;
	m[3] = 0;
	m[4] = 0;
	m[5] = e;
	m[6] = 0;
	m[7] = 0;
	m[8] = 0;
	m[9] = 0;
	m[10] = e;
	m[11] = 0;
	m[12] = e;
	m[13] = e;
	m[14] = e;
	m[15] = e;
}

/*
	n 0 0 0
	0 n 0 0
	0 0 n 0
	0 0 0 1
*/
void		mat4_scale(mat4 m, float scale)
{
	m[0] *= scale;
	m[5] *= scale;
	m[10] *= scale;
}

void		mat4_inverse(mat4 m)
{
	mat4	tmp;

	tmp[0] = m[0];
	tmp[4] = m[1];
	tmp[8] = m[2];
	tmp[12] = 0;
	tmp[1] = m[4];
	tmp[5] = m[5];
	tmp[9] = m[6];
	tmp[13] = 0;
	tmp[2] = m[8];
	tmp[6] = m[9];
	tmp[10] = m[10];
	tmp[14] = 0;
	tmp[3] = -(m[3] * tmp[0] + m[7] * tmp[1] + m[11] * tmp[2]);
	tmp[7] = -(m[3] * tmp[4] + m[7] * tmp[5] + m[11] * tmp[6]);
	tmp[11] = -(m[3] * tmp[8] + m[7] * tmp[9] + m[11] * tmp[10]);
	tmp[15] = 1;
	memcpy(m, tmp, sizeof(mat4));
}

void		mat4_print(mat4 m)
{
	unsigned int	i;

	for (i = 0; i < 4; i++) {
		// printf("[ %.4f | %.4f | %.4f | %.4f ]\n", m[0 + i], m[4 + i], m[8 + i], m[12 + i]);
		printf("[ %.4f | %.4f | %.4f | %.4f ]\n", m[0 + i * 4], m[1 + i * 4], m[2 + i * 4], m[3 + i * 4]);
	}
	printf("\n");
}

/*
	1 0 0 x
	0 1 0 y
	0 0 1 z
	0 0 0 1
*/
void		mat4_translate(mat4 m, float x, float y, float z)
{
	m[3] += x;
	m[7] += y;
	m[11] += z;
}
