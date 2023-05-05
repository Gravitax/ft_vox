#ifndef LIB_MAT_VEC_H
# define LIB_MAT_VEC_H


# include <math.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <string.h>

# define MV_PI	3.14159

typedef float	mat4[16];

typedef struct	s_vec3
{
	float		x, y, z;
}				vec3;

typedef union	rsqrt
{
	float		f;
	uint32_t	i;
}				u_rsqrt;

vec3		vec_fdiv(vec3 a, float n);
vec3		vec_fmult(vec3 a, float n);
vec3		vec_fadd(vec3 a, float n);
vec3		vec_fsub(vec3 a, float n);

vec3		vec_cross(vec3 a, vec3 b);

vec3		vec_add(vec3 a, vec3 b);
vec3		vec_sub(vec3 a, vec3 b);
vec3		vec_mult(vec3 a, vec3 b);
vec3		vec_div(vec3 a, vec3 b);

float		ft_rsqrt(float number);
vec3		vec_normalize(vec3 vec);
float		vec_dot(vec3 a, vec3 b);
float		vec3d_dist(vec3 o, vec3 v);
bool		vec_is_zero(vec3 v);

// matrices
void		mat4_multiply(mat4 a, mat4 b);
vec3		mat4_x_vec3(mat4 m, vec3 v);

void		mat4_xrotation(mat4 m, float x);
void		mat4_yrotation(mat4 m, float y);
void		mat4_zrotation(mat4 m, float z);
void		mat4_rotate(mat4 m, float x, float y, float z);

void		mat4_identity(mat4 m);
void		mat4_bias(mat4 m, float e);
void		mat4_scale(mat4 m, float scale);
void		mat4_inverse(mat4 m);
void		mat4_print(mat4 m);
void		mat4_translate(mat4 m, float x, float y, float z);

void		mat4_projection(mat4 m, float fov, float near, float far, float ratio);
void		mat4_orthogonal(mat4 m, float top, float right, float bottom, float left, float near, float far);
void		mat4_lookat(mat4 m, vec3 from, vec3 to, vec3 world_up);


#endif
