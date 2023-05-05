#include "../include/main.h"


static void				render_mesh(t_mesh *mesh);

// ====================================================================

static void				shadows_options(t_env *env, int i)
{
	env->light.current = i;
	// we mount a view and proj matrix for each light source
	mat4_lookat(env->model.depthview[i],
		env->light.sources[i].pos,
		vec_add(env->light.sources[i].pos, env->light.sources[i].dir),
		(vec3){ 0, 1, 0 });
	mat4_inverse(env->model.depthview[i]);
	mat4_projection(env->model.depthproj[i],
		env->light.sources[i].fov,
		env->light.sources[i].near,
		env->light.sources[i].far,
		env->light.sources[i].ratio);
}

static void				update_options(t_env *env, char type)
{
	switch (type) {
		case (0): // depth
			reset_viewport(env->window.ptr, DEPTHMAP_W, DEPTHMAP_H);
			glClear(GL_DEPTH_BUFFER_BIT);
			glCullFace(GL_FRONT);
			// Passes if the incoming depth value is less than the stored depth value.
			glDepthFunc(GL_LESS);
			break;
		case (1): // model
			reset_viewport(env->window.ptr, env->window.w, env->window.h);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glCullFace(GL_BACK);
			// Passes if the incoming depth value is less than the stored depth value.
			glDepthFunc(GL_LESS);
			// update uniforms
			set_uniforms(env, 1);
			break;
		case (2): // skybox
			glCullFace(GL_FRONT);
			// Passes if the incoming depth value is less than or equal to the stored depth value.
			glDepthFunc(GL_LEQUAL);
			// update uniforms
			set_uniforms(env, 2);
			break;
	}
}

// ====================================================================

static void				pass_depth(t_env *env)
{
	int		i, j;
	t_mesh	*mesh;

	glBindFramebuffer(GL_FRAMEBUFFER, env->model.fbo);

	update_options(env, 0);
	i = SHADOW_SOURCE_SUN;
	//while (++i < SHADOW_SOURCE_MAX) {
		// calculate new view / proj matrix for each lightsource
		shadows_options(env, i);
		// update uniforms
		set_uniforms(env, 0);
		j = -1;
		while (++j < env->model.meshs.nb_cells - 1) {
			mesh = dyacc(&env->model.meshs, j);
			render_mesh(mesh);
		}
	//}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

static void				pass_scene(t_env *env)
{
	int		i;
	bool	skybox;
	t_mesh	*mesh;

	update_options(env, 1);
	i = -1;
	while (++i < env->model.meshs.nb_cells - 1) {
		mesh = dyacc(&env->model.meshs, i);
		render_mesh(mesh);
	}
}

static inline void		pass_skybox(t_env *env)
{
	update_options(env, 2);
	render_mesh(dyacc(&env->model.meshs, env->model.meshs.nb_cells - 1));
}

// ====================================================================

static inline void		mat4_view(t_camera *camera)
{
	if (camera->ground_fixed == true)
		camera->pos.y = 0;
	mat4_lookat(camera->view, camera->pos, vec_add(camera->pos, camera->zaxis), camera->yaxis);
	mat4_inverse(camera->view);
}

static inline void		mat4_model(t_model *model)
{
	mat4_identity(model->model);
	mat4_scale(model->model, model->scale);
}

static void				update_data(t_env *env)
{
	// FPS
	// display :: true / false
	fps(&env->fps, true);

	// MODEL
	mat4_model(&env->model);
	mat4_view(&env->camera);
	mat4_projection(env->camera.projection, env->camera.fov, env->camera.near, env->camera.far, env->camera.ratio);
	// we update the model center
	const float	e = SQUARE_SIZE / 2.0f * CHUNK_SIZE;

	env->model.center = (vec3){ e + env->model.chunks[0][0].x_start, 0, e + env->model.chunks[0][0].z_start};

	// LIGHT
	// we update the flaslight position
	env->light.sources[LIGHT_SOURCE_PLAYER].pos = env->camera.pos;
	env->light.sources[LIGHT_SOURCE_PLAYER].dir = env->camera.zaxis;
	// player's height
	env->light.sources[LIGHT_SOURCE_PLAYER].pos.y -= MODEL_SCALE / 2.0f;
	// sunlight is updated in uniforms.c
}

// ====================================================================

static void				render_mesh(t_mesh *mesh)
{
	glBindVertexArray(mesh->vao);
	glDrawArrays(GL_TRIANGLES, 0, mesh->vertices.nb_cells);
	glBindVertexArray(0);
}

static unsigned char	render_scene(t_env *env)
{
	unsigned char	code;

	// update data
	update_data(env);
	if ((code = update_world(env)) != ERR_NONE)
		return (code);
	// first  - render depth
	if (env->light.shadow == true)
		pass_depth(env);
	// second - render scene
	pass_scene(env);
	// third  - render skybox
	pass_skybox(env);
	return (ERR_NONE);
}

// ====================================================================

unsigned char			display_loop(t_env *env)
{
	unsigned char	code;

	glClearColor(DEFAULT_COLOR.x, DEFAULT_COLOR.y, DEFAULT_COLOR.z, 1);
	while (!glfwWindowShouldClose(env->window.ptr))
	{
		// handle inputs
		processInput(env->window.ptr);
		// render scene
		if ((code = render_scene(env)) != ERR_NONE)
			return (code);
		// glfw: swap buffers and poll IO events
		glfwSwapBuffers(env->window.ptr);
		glfwPollEvents();
	}
	return (ERR_NONE);
}
