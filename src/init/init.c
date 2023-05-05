#include "../../include/main.h"

static void				bind_actions_to_keys(t_env *env)
{
	// Function pointers array used to synthetize actions function
	static void	(*keys_fts[NB_KEYS])(t_env *env, int key) = {
		[KEY_EXIT] = exit_vox,
		[KEY_MOVE_CAM_FORWARD] = move_cam,
		[KEY_MOVE_CAM_BACKWARD] = move_cam,
		[KEY_MOVE_CAM_UP] = move_cam,
		[KEY_MOVE_CAM_DOWN] = move_cam,
		[KEY_MOVE_CAM_LEFT] = move_cam,
		[KEY_MOVE_CAM_RIGHT] = move_cam,
		[KEY_LIGHT] = event_light,
		[KEY_SHADOW] = event_shadow,
	};

	// Assignation of actions functions in respect with handled key indices.
	for (unsigned int i = 0; i < KEY_MAX; i++)
		env->keybinds_fts[env->settings.keys[i]] = keys_fts[i];
}

static void				gl_options(void)
{
	// DEPTH BUFFER
	glEnable(GL_DEPTH_TEST);

	// CULLING
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	// GAMA CORRECTION
	// glEnable(GL_FRAMEBUFFER_SRGB);

	// ANTI ALIASING
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_LINE_SMOOTH);

	// BLENDING
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
}

unsigned char			init(t_env *env, int argc, char **argv)
{
	unsigned char code;

	srand(time(NULL));
	env->window.fullscreen = true;

	if ((code = load_settings(env)) != ERR_NONE // Loads settings data from Settings.toml
		|| (code = load_textures(env)) != ERR_NONE
		|| (code = load_shaders(env)) != ERR_NONE
		|| (code = init_display(env)) != ERR_NONE // Inits display with glad and glfw3
		|| (code = init_world(env, argc, argv)) != ERR_NONE
		|| (code = init_meshs(env)) != ERR_NONE)  // init shaders after model because we need to buffer each mesh
		return (code);

	bind_actions_to_keys(env); // Attribute action functions to keys loaded from settings file

	gl_options();

	return (ERR_NONE);
}
