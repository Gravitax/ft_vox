#ifndef MODEL_H
# define MODEL_H


# include "main.h"
# include "gen.h"
# include "light.h"

# define MODEL_SCALE	1
# define SB_ROT_SPEED	5 // sun's rotation speed
# define DEPTHMAP_W		1024
# define DEPTHMAP_H		1024

enum			e_texture
{
	TEXTURE_WATER,
	TEXTURE_SAND,
	TEXTURE_GRASS,
	TEXTURE_GROUND,
	TEXTURE_STONE,
	TEXTURE_SNOW,
	TEXTURE_GRASS_SIDE,
	TEXTURE_SNOW_SIDE,
	TEXTURE_MAX
};

enum			e_skybox
{
	TEXTURE_SB_PX = TEXTURE_MAX,
	TEXTURE_SB_PY,
	TEXTURE_SB_PZ,
	TEXTURE_SB_NX,
	TEXTURE_SB_NY,
	TEXTURE_SB_NZ,
	TEXTURE_SB_MAX
};

enum			e_normals
{
	N_NORTH,
	N_SOUTH,
	N_WEST,
	N_EAST,
	N_UP,
	N_DOWN,
	N_MAX
};

typedef	struct	s_vertex_texture
{
	float	u, v;
}				t_vt;

typedef struct __attribute__((__packed__)) s_stride
{
	short	x : 16;
	short	y : 16;
	short	z : 16;
	int		uv : 3;
	int		fs : 6; // fall_size
	int		n : 3;
	int		bt : 3;
	int		floor : 1;
}				__attribute__((__packed__)) t_stride;

typedef struct	s_texture
{
	unsigned char	*ptr;
	unsigned int	w, h;
}				t_texture;

typedef struct	s_uniform
{
	GLint	time, campos;
	GLint	textures[TEXTURE_MAX], skybox, depthmap;
	GLint	light_active, shadow, light_gamma, light[LIGHT_SOURCE_MAX][LIGHT_MAX];
	// mvp for program
	GLint	model, view, projection;
	// mvp for program_skybox
	GLint	sbmodel, sbview, sbprojection;
	// mvp for program_depth
	GLint	dview, dproj;
}				t_uniform;

typedef struct	s_mesh
{
	t_dynarray	vertices;
	// vertex array object, vertex buffer object
	GLuint		vao, vbo;
	int			x_start, z_start;
	bool		removed;
}				t_mesh;

typedef struct	s_model
{
	t_dynarray	meshs;
	GLuint		program, program_skybox, program_depth; // shaders' program for model, skybox and depth
	t_uniform	uniforms; // 
	int			square_x, square_z;
	t_chunk		chunks[SQUARE_SIZE][SQUARE_SIZE];
	mat4		model, depthproj[LIGHT_SOURCE_MAX], depthview[LIGHT_SOURCE_MAX];
	float		scale;
	GLuint		gl_textures[TEXTURE_MAX];		// gl's textures' id
	GLuint		gl_tskybox;						// gl's texture for skybox
	GLuint		depthmap;						// gl's texture for depthmap
	GLuint		fbo;							// frame buffer object
	t_texture	textures[TEXTURE_SB_MAX];		// textures' ptr
	vec3		center;
}				t_model;

#endif
