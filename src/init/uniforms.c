#include "../../include/main.h"


static void				light_and_shadows_uniforms(t_env *env, mat4 m)
{
	// LIGHT PLAYER
	glUniform3fv(env->model.uniforms.light[LIGHT_SOURCE_PLAYER][LIGHT_POSITION], 1, (GLfloat *)&env->light.sources[LIGHT_SOURCE_PLAYER].pos);
	glUniform3fv(env->model.uniforms.light[LIGHT_SOURCE_PLAYER][LIGHT_DIRECTION], 1, (GLfloat *)&env->light.sources[LIGHT_SOURCE_PLAYER].dir);

	// SUNLIGHT
	// sunlight follow the sun's texture :: we make it rotate like the skybox
	env->light.sources[LIGHT_SOURCE_SUN].pos = mat4_x_vec3(m, env->light.sources[LIGHT_SOURCE_SUN].base_pos);
	glUniform3fv(env->model.uniforms.light[LIGHT_SOURCE_SUN][LIGHT_POSITION], 1, (GLfloat *)&env->light.sources[LIGHT_SOURCE_SUN].pos);
	// we update sunlight dir
	env->light.sources[LIGHT_SOURCE_SUN].dir = vec_add(env->light.sources[LIGHT_SOURCE_SUN].pos, env->model.center);
	// env->light.sources[LIGHT_SOURCE_SUN].dir = vec_normalize(env->light.sources[LIGHT_SOURCE_SUN].dir);
	glUniform3fv(env->model.uniforms.light[LIGHT_SOURCE_SUN][LIGHT_DIRECTION], 1, (GLfloat *)&env->light.sources[LIGHT_SOURCE_SUN].dir);
	if (env->light.shadow == true) {
		// SHADOWS :: update depth matrices
		// we set a proj and view matrix for each lightsource (not scalable yet)
		glUniformMatrix4fv(glGetUniformLocation(env->model.program, "dvp"), 1, GL_FALSE, env->model.depthview[0]);
		glUniformMatrix4fv(glGetUniformLocation(env->model.program, "dpp"), 1, GL_FALSE, env->model.depthproj[0]);
		glUniformMatrix4fv(glGetUniformLocation(env->model.program, "dvs"), 1, GL_FALSE, env->model.depthview[1]);
		glUniformMatrix4fv(glGetUniformLocation(env->model.program, "dps"), 1, GL_FALSE, env->model.depthproj[1]);
	}
}

/*
	update uniforms for each program
*/
void					set_uniforms(t_env *env, char type)
{
	mat4	m;

	// rotation and translation around player (for sunlight and skybox for exemple)
	mat4_identity(m);
	mat4_translate(m, env->camera.pos.x, env->camera.pos.y, env->camera.pos.z);
	mat4_yrotation(m, (env->fps.current_seconds * SB_ROT_SPEED) / 100.0f);

	// ----------------------------------

	switch (type) {
		case (0): // depth
			glUseProgram(env->model.program_depth);
			// update matrices
			glUniformMatrix4fv(env->model.uniforms.dview, 1, GL_FALSE, env->model.depthview[env->light.current]);
			glUniformMatrix4fv(env->model.uniforms.dproj, 1, GL_FALSE, env->model.depthproj[env->light.current]);
			break ;
		case (1): // model
			glUseProgram(env->model.program);
			// update time
			glUniform1f(env->model.uniforms.time, env->fps.current_seconds);
			// update campos
			glUniform3fv(env->model.uniforms.campos, 1, (GLfloat *)&env->camera.pos);
			if (env->light.is_active == true)
				light_and_shadows_uniforms(env, m);
			// update matrices
			glUniformMatrix4fv(env->model.uniforms.model, 1, GL_FALSE, env->model.model);
			glUniformMatrix4fv(env->model.uniforms.view, 1, GL_FALSE, env->camera.view);
			glUniformMatrix4fv(env->model.uniforms.projection, 1, GL_FALSE, env->camera.projection);
			break ;
		case (2): // skybox
			glUseProgram(env->model.program_skybox);
			// skybox's rotation around the player
			mat4_multiply(env->model.model, m);
			// update matrices
			glUniformMatrix4fv(env->model.uniforms.sbmodel, 1, GL_FALSE, env->model.model);
			glUniformMatrix4fv(env->model.uniforms.sbview, 1, GL_FALSE, env->camera.view);
			glUniformMatrix4fv(env->model.uniforms.sbprojection, 1, GL_FALSE, env->camera.projection);
			break ;
	}
}

// =======================================================================

/*
	we stock uniform's id for later use (matrices, ...)
	and we consume what we need (textures, light, ...)
*/
unsigned char			load_uniforms(t_env *env, char type)
{
	unsigned char	code;
	GLuint			program;

	switch (type) {
		case (0): // depthmap
			program = env->model.program_depth;
			glUseProgram(program);
			// mvp
			env->model.uniforms.dview = glGetUniformLocation(program, "view");
			env->model.uniforms.dproj = glGetUniformLocation(program, "projection");
			break ;
		case (1): // model
			program = env->model.program;
			glUseProgram(program);
			// mvp
			env->model.uniforms.model = glGetUniformLocation(program, "model");
			env->model.uniforms.view = glGetUniformLocation(program, "view");
			env->model.uniforms.projection = glGetUniformLocation(program, "projection");
			// time
			env->model.uniforms.time = glGetUniformLocation(program, "u_time");
			// campos
			env->model.uniforms.campos = glGetUniformLocation(program, "campos");
			// we set textures and light
			if ((code = textures_uniforms(env)) != ERR_NONE
					|| (code = light_uniforms(env)) != ERR_NONE)
				return (code);
			break ;
		case (2): // skybox
			program = env->model.program_skybox;
			glUseProgram(program);
			// mvp
			env->model.uniforms.sbmodel = glGetUniformLocation(program, "model");
			env->model.uniforms.sbview = glGetUniformLocation(program, "view");
			env->model.uniforms.sbprojection = glGetUniformLocation(program, "projection");
			// texture
			env->model.uniforms.skybox = glGetUniformLocation(program, "vSkybox");
			// we set the texture
			glUniform1i(env->model.uniforms.skybox, 0);
			break ;
	}
	return (ERR_NONE);
}
