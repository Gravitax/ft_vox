#include "main.h"


void		fps(t_fps *fps, bool print)
{
	fps->current_seconds = glfwGetTime();
	fps->elapsed_seconds = fps->current_seconds - fps->time;
	if (fps->elapsed_seconds > 1) {
		fps->time = fps->current_seconds;
		fps->value = (double)fps->frames / fps->elapsed_seconds;
		fps->frames = 0;

		print ? printf("fps: %d\n", fps->value) : 0;
	}

	fps->delta = fps->current_seconds - fps->lastframe;
	fps->lastframe = fps->current_seconds;
	++fps->frames;
}
