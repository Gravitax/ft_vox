#include "../../include/main.h"

static GLFWwindow*		create_window(const char *title, int width, int height, bool fullscreen)
{
	GLFWmonitor		 	*monitor;
	const GLFWvidmode   *mode;

	if (fullscreen)
	{
		monitor = glfwGetPrimaryMonitor();
		mode = glfwGetVideoMode(monitor);
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
		return (glfwCreateWindow(mode->width, mode->height, title, monitor, NULL));
	}
	return (glfwCreateWindow(width, height, title, NULL, NULL));
}

static unsigned char	glfw_create_window(GLFWwindow* *window, const char *title, int width, int height, bool fullscreen)
{
	// Setting a Minimum OpenGL Version to Use
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Anti-Aliasing built-in "smoothing"
	// The more "samples" or passes it does, the more smoothed it will look, but it gets more expensive. Set it to "16" before taking screen shots!
	glfwWindowHint(GLFW_SAMPLES, 4);
	*window = create_window(title, width, height, fullscreen);
	return(*window ? ERR_NONE : ERR_FAILED_WIN);
}

unsigned char			init_display(t_env *env)
{
	unsigned char	code;

	// glfwSetErrorCallback(cb_error);
	if (!glfwInit() || !GL_VERSION_2_1)
		return (ERR_GLFW_INIT);

#ifdef __APPLE__
	// Calling glfwInit changes current working directory of our process to './resources'
	if (chdir("../") == -1)
	{
		perror(strerror(errno));
		return (ERR_CHDIR_FAILED);
	}
#endif

	if ((code = glfw_create_window(&env->window.ptr, "ft_vox",
			env->window.w, env->window.h, env->window.fullscreen) != ERR_NONE))
		return (code);

	glfwMakeContextCurrent(env->window.ptr);

	if (!gladLoadGL())
		return (ERR_FAILED_GLAD);

	glfwSwapInterval(1);
	glfw_init_callbacks(env);
	env->fps.time = glfwGetTime();
	return (ERR_NONE);
}
