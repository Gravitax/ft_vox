#include "../../include/main.h"


__attribute__((noreturn)) void	exit_vox(t_env *env, int key)
{
	(void)key;

	free_env(env);
	exit(EXIT_SUCCESS);
}

void		event_light(t_env *env, int key)
{
	(void)key;

	// we reset shadow if light is on or off
	env->light.shadow = false;
	env->light.is_active = !env->light.is_active;
	// use program before update uniforms
	glUseProgram(env->model.program);
	// we update the uniforms
	glUniform1i(env->model.uniforms.shadow, env->light.shadow);
	glUniform1i(env->model.uniforms.light_active, env->light.is_active);
}

void		event_shadow(t_env *env, int key)
{
	(void)key;

	// si les lights sont désactivées alors shadow est mis à false
	env->light.shadow = env->light.is_active ? !env->light.shadow : false;
	// use program before update uniforms
	glUseProgram(env->model.program);
	// we update the uniforms
	glUniform1i(env->model.uniforms.shadow, env->light.shadow);
}
