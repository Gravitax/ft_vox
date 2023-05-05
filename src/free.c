#include "main.h"

void		free_hmap(uint8_t **hmap)
{
	for (unsigned int i = 0; i < CHUNK_SIZE; i++)
		free(hmap[i]);
	free(hmap);
}

void		free_cave_map(uint8_t ***cave_map)
{
	if (!cave_map)
		return ;
	for (int i = 0; i < CHUNK_SIZE; i++)
	{
		for (int j = 0; j < CAVE_DEPTH; j++)
			free(cave_map[i][j]);

		free(cave_map[i]);
	}
	free(cave_map);
}

static void	free_shaders(t_env *env)
{
	for (int i = 0; i < SHADER_MAX; i++)
		munmap((void*)env->shaders[i].source, env->shaders[i].size);
}

static void	free_textures(t_env *env)
{
	for (unsigned int i = 0; i < TEXTURE_SB_MAX; i++)
		if (env->model.textures[i].ptr)
			free(env->model.textures[i].ptr);

	if (env->model.gl_textures[0] != 0)
	{
		glDeleteTextures(TEXTURE_MAX + 1, env->model.gl_textures);
		glDeleteTextures(1, &env->model.gl_tskybox);
	}
}

static void	free_chunk(t_chunk *chunk)
{
	if (chunk->surface_hmap)
		free_hmap(chunk->surface_hmap);

	if (chunk->cave_map)
		free_cave_map(chunk->cave_map);

	if (chunk->stride.nb_cells > 0)
		dynarray_free(&chunk->stride);
}

static void	free_chunks(t_env *env)
{
	t_mesh	*m;

	for (int x = 0; x < SQUARE_SIZE; x++)
		for (int z = 0; z < SQUARE_SIZE; z++)
			free_chunk(&env->model.chunks[x][z]);

	for (int i = 0; i < env->model.meshs.nb_cells; i++)
	{
		m = dyacc(&env->model.meshs, i);

		if (i == env->model.meshs.nb_cells - 1) // Skybox
			dynarray_free(&m->vertices);

		glDeleteBuffers(1, &m->vao);
		glDeleteBuffers(1, &m->vbo);
	}

	if (env->model.fbo != 0)
		glDeleteBuffers(1, &env->model.fbo);

	if (env->model.meshs.byte_size > 0)
		dynarray_free(&env->model.meshs);
}

void		free_env(t_env *env) 
{
	free_chunks(env);
	free_textures(env);
	free_shaders(env);

	glfwDestroyWindow(env->window.ptr);
	glfwTerminate();
}
