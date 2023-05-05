#ifndef SHADERS_H
# define SHADERS_H


enum e_shaders_ids
{
	SHADER_VERTEX,
	SHADER_FRAGMENT,
	SHADER_SB_VERTEX,
	SHADER_SB_FRAGMENT,
	SHADER_DEPTH_VERTEX,
	SHADER_DEPTH_FRAGMENT,
	SHADER_MAX
};

// Paths array to shaders source files
static const char	*shaders_path[SHADER_MAX] = {
	[SHADER_VERTEX]			= "src/shaders/vertex.glsl",
	[SHADER_FRAGMENT]		= "src/shaders/fragment.glsl",
	[SHADER_SB_VERTEX]		= "src/shaders/skybox_vertex.glsl",
	[SHADER_SB_FRAGMENT]	= "src/shaders/skybox_fragment.glsl",
	[SHADER_DEPTH_VERTEX]	= "src/shaders/depth_vertex.glsl",
	[SHADER_DEPTH_FRAGMENT]	= "src/shaders/depth_fragment.glsl"
};

typedef struct		s_shaders
{
	const GLchar	*source;
	size_t			size;
	int				id;
}					t_shaders;


#endif
