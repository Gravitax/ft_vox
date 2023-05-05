#include "../../include/main.h"


void		move_cam(t_env *env, int key)
{
	t_camera	*camera;
	float		speed;

	camera = &env->camera;
	speed = camera->sprint ? camera->tspeed : camera->speed;
	speed *= env->fps.delta;

	if (key == GLFW_KEY_D)
		camera->pos = vec_add(camera->pos, vec_fmult(vec_cross(camera->zaxis, camera->yaxis), speed));
	if (key == GLFW_KEY_A)
		camera->pos = vec_sub(camera->pos, vec_fmult(vec_cross(camera->zaxis, camera->yaxis), speed));
	if (key == GLFW_KEY_W)
		camera->pos = vec_add(camera->pos, vec_fmult(camera->zaxis, speed));
	if (key == GLFW_KEY_S)
		camera->pos = vec_sub(camera->pos, vec_fmult(camera->zaxis, speed));
	if (key == GLFW_KEY_R)
		camera->pos = vec_add(camera->pos, vec_fmult(camera->yaxis, speed));
	if (key == GLFW_KEY_F)
		camera->pos = vec_sub(camera->pos, vec_fmult(camera->yaxis, speed));
}

static void update_directions(t_camera *camera)
{
	camera->zaxis.x = -sinf((float)ft_to_radians(camera->pitch)) * cosf((float)ft_to_radians(camera->yaw));
	camera->zaxis.y = sinf((float)ft_to_radians(camera->yaw));
	camera->zaxis.z = cosf((float)ft_to_radians(camera->pitch)) * cosf((float)ft_to_radians(camera->yaw));
}

static void	wrap_angles(t_camera *camera)
{
	camera->pitch = camera->pitch > 360 ? 0 : camera->pitch;
	camera->pitch = camera->pitch < 0 ? 360 : camera->pitch;
	if (camera->ground_fixed == true) {
		camera->yaw = camera->yaw > 45 ? 45 : camera->yaw;
		camera->yaw = camera->yaw < -45 ? -45 : camera->yaw;
	} else {
		camera->yaw = camera->yaw > 89 ? 89 : camera->yaw;
		camera->yaw = camera->yaw < -89 ? -89 : camera->yaw;
	}
}

static void	update_angles(t_camera *camera, float sensitivity, float xoffset, float yoffset)
{
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	camera->pitch += xoffset;
	camera->yaw += yoffset;
	wrap_angles(camera);
}

void		events_mouse(t_env *env, float xpos, float ypos)
{
	t_mouse	*mouse;
	float	xoffset, yoffset;

	mouse = &env->mouse;
	xoffset = xpos - mouse->pos.x;
	yoffset = mouse->pos.y - ypos;
	mouse->pos.x = xpos;
	mouse->pos.y = ypos;
	update_angles(&env->camera, mouse->sensitivity * 0.005f, xoffset, yoffset);
	update_directions(&env->camera);
}
