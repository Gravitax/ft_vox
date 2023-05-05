#include "lib_mat_vec.h"


vec3		vec_add(vec3 a, vec3 b)
{
	vec3	ret;

	ret.x = a.x + b.x;
	ret.y = a.y + b.y;
	ret.z = a.z + b.z;
	return (ret);
}

vec3		vec_sub(vec3 a, vec3 b)
{
	vec3	ret;

	ret.x = a.x - b.x;
	ret.y = a.y - b.y;
	ret.z = a.z - b.z;
	return (ret);
}

vec3		vec_mult(vec3 a, vec3 b)
{
	vec3	ret;

	ret.x = a.x * b.x;
	ret.y = a.y * b.y;
	ret.z = a.z * b.z;
	return (ret);
}

vec3		vec_div(vec3 a, vec3 b)
{
	vec3	ret;

	ret.x = a.x / b.x;
	ret.y = a.y / b.y;
	ret.z = a.z / b.z;
	return (ret);
}
