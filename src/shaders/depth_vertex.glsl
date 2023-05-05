#version 400 core

layout (location = 0) in int	data1;
layout (location = 1) in int	data2;

uniform mat4 					view;
uniform mat4 					projection;

void	main()
{
	vec4		pos;

	//////// Bitfield unpacking ///////////
	pos.x = float(data1 & 0xFFFF);
	pos.y = float((data1 & 0xFFFF0000) >> 16u);
	pos.z = float(data2 & 0xFFFF);
	pos.w = 1.0f;
	//////////////////////////////////////

	gl_Position	= pos * view * projection;
}
