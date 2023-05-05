#include "lib_mat_vec.h"


/*
	e = 1 / tan(fov / 2 ∗ π / 180)
	a = (far + near) / (far - near)
	b = (2 * far * near) / (far - near)

	e 0 0 0
	0 e 0 0
	0 0 -a -1
	0 0 -b 0
*/ 
void		mat4_projection(mat4 m, float fov, float near, float far, float ratio)
{
	float	e;

	mat4_identity(m);
	e = 1 / tanf(fov * 0.5f * MV_PI / 180);

	m[0] = e / ratio;
	m[5] = e;
	m[10] = -(far + near) / (far - near);
	m[11] = -2 * far * near / (far - near);
	m[14] = -1;
	m[15] = 0;
}

void		mat4_orthogonal(mat4 m, float top, float right, float bottom, float left, float near, float far)
{
	mat4_identity(m);
	m[0] = 2.0f / (right - left);
	m[5] = 2.0f / (top - bottom);
	m[10] = -2.0f / (far - near);
	m[12] = -(right + left) / (right - left);;
	m[13] = -(top + bottom) / (top - bottom);;
	m[14] = -(far + near) / (far - near);;
	m[15] = 1;
}

/*
	zaxis = normal(At - Eye)
	xaxis = normal(cross(Up, zaxis))
	yaxis = cross(zaxis, xaxis)

	xaxis.x				yaxis.x				zaxis.x				0
	xaxis.y				yaxis.y				zaxis.y				0
	xaxis.z				yaxis.z				zaxis.z				0
	-dot(xaxis, eye)	-dot(yaxis, eye)	-dot(zaxis, eye)	1
*/
void		mat4_lookat(mat4 m, vec3 from, vec3 to, vec3 world_up)
{
	vec3	forward, up, right;

	forward = vec_normalize(vec_sub(from, to));
	right = vec_normalize(vec_cross(world_up, forward));
	up = vec_cross(forward, right);

	m[0] = right.x;
	m[4] = right.y;
	m[8] = right.z;
	m[12] = -vec_dot(right, from);
	m[1] = up.x;
	m[5] = up.y;
	m[9] = up.z;
	m[13] = -vec_dot(up, from);
	m[2] = forward.x;
	m[6] = forward.y;
	m[10] = forward.z;
	m[14] = -vec_dot(forward, from);
	m[3] = from.x;
	m[7] = from.y;
	m[11] = from.z;
	m[15] = 1;
}
