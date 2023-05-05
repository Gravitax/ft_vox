#include "main.h"


void		reset_viewport(GLFWwindow * window, unsigned int width, unsigned int height)
{

	int	w, h, e = 1;
	glfwGetWindowSize(window, &w, &h);
	width = w;
	height = h;

	#ifdef __APPLE__
		e = 2;
	#endif

	glViewport(0, 0, width * e, height * e); // Maybe e should be our aspect ratio
}
