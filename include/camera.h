#ifndef CAMERA_H
# define CAMERA_H


# include "./main.h"

typedef struct	s_camera
{
	mat4		view, projection;
	float		pitch, yaw;
	float		fov, near, far, ratio;
	vec3		pos, yaxis, zaxis;
	float		speed, tspeed;
	bool		ground_fixed;
	bool		sprint;
}				t_camera;


#endif
