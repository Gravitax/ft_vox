#version 400 core

layout (location = 0) in vec3	aPosition;

uniform mat4 					model;
uniform mat4 					view;
uniform mat4 					projection;

out vec3						vPosition;

void	main()
{
	vec4	pos	= vec4(aPosition, 1) * model * view * projection;

	vPosition	= aPosition;

	// z is useless for the skybox
	gl_Position	= pos.xyww;
}
