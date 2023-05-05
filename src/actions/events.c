# include "main.h"


enum					e_toggles
{
	TOGGLE_EXIT,
	TOGGLE_LIGHT,
	TOGGLE_SHADOW,
	TOGGLE_MAX
};

static t_env *g_env;

// ====================================================================

static bool				switch_toggles(t_env *env, bool toggles[TOGGLE_MAX], int key, bool press)
{
	uint8_t	*keys = &env->settings.keys[0];
	int		toggle_ids[TOGGLE_MAX] = {
		gl_keys_values[env->settings.keys[KEY_EXIT]],
		gl_keys_values[env->settings.keys[KEY_LIGHT]],
		gl_keys_values[env->settings.keys[KEY_SHADOW]],
	};

	for (unsigned int i = 0; i < TOGGLE_MAX; i++)
		if (key == toggle_ids[i])
		{
			if (toggles[i] && press) {
				return (false);
			}
			toggles[i] = press;
			return (true);
		}
	return (true);
}

void					processInput(GLFWwindow *window)
{
	static bool	toggles[TOGGLE_MAX] = { false, false};

	g_env->camera.sprint = glfwGetKey(g_env->window.ptr, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
	for (int i = 0; i < NB_KEYS; i++) // Iterate through every keys
	{
		if (glfwGetKey(window, gl_keys_values[i]) == GLFW_PRESS // Is the key pressed ?
			&& switch_toggles(g_env, toggles, gl_keys_values[i], true) // If the key is a toggle, is it released ?
			&& g_env->keybinds_fts[i]) // Is a function associated with the key ?
			g_env->keybinds_fts[i](g_env, gl_keys_values[i]); // Then let's launch it
		if (glfwGetKey(window, gl_keys_values[i]) == GLFW_RELEASE // Is the key released ?
			&& g_env->keybinds_fts[i]) // Is a function associated with the key ?
			switch_toggles(g_env, toggles, gl_keys_values[i], false); // Makes toggles available again.
	}
}

// ====================================================================

static void				cb_cursor_position(GLFWwindow *window, double xpos, double ypos)
{
	glfwGetCursorPos(window, &xpos, &ypos);
	events_mouse(g_env, (float)xpos, (float)ypos);
}

static void				cb_framebuffer_size(GLFWwindow *window, int width, int height)
{
	(void)window;

	reset_viewport(window, width, height);

	g_env->settings.w_wdt = (uint16_t)width;
	g_env->settings.w_hgt = (uint16_t)height;

	g_env->window.w = width;
	g_env->window.h = height;
	g_env->camera.ratio = (float)width / (float)height;

}

static float			clamp(float min, float max, float val)
{
	float	delta;

	delta = max - min;
	return (val / delta);
}

static void				cb_scroll(GLFWwindow *window, double xoffset, double yoffset)
{
	(void)window;
	(void)xoffset;
	g_env->camera.fov -= (float)yoffset;
	g_env->camera.fov = g_env->camera.fov > 90 ? 90 : g_env->camera.fov;
	g_env->camera.fov = g_env->camera.fov < 1 ? 1 : g_env->camera.fov;
	g_env->mouse.sensitivity = g_env->mouse.base_sensitivity * clamp(1.0f, 90.0f, g_env->camera.fov);
}

// ====================================================================

void 					glfw_init_callbacks(t_env *env)
{
	g_env = env;
	glfwSetCursorPosCallback(env->window.ptr, cb_cursor_position);
	glfwSetFramebufferSizeCallback(env->window.ptr, cb_framebuffer_size);
	glfwSetScrollCallback(env->window.ptr, cb_scroll);
}
