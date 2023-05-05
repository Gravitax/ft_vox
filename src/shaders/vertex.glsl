#version 400 core

layout (location = 0) in int	data1;
layout (location = 1) in int	data2;

uniform mat4 					model;
uniform mat4 					view;
uniform mat4 					projection;
uniform mat4 					dvp;
uniform mat4 					dpp;
uniform mat4 					dvs;
uniform mat4 					dps;
uniform float 					u_time;

out vec3						vNormal;
out vec3						vPosition;
out vec2						vTextCoord;
out vec4						vShadCoordP;
out vec4						vShadCoordS;
flat out float					vType;

vec3	get_normal(int n)
{
	vec3	normal = vec3(0);

	switch (n) {
		case (0):
			normal = vec3(0, 0, 1);
			break ;
		case (1):
			normal = vec3(0, 0, -1);
			break ;
		case (2):
			normal = vec3(-1, 0, 0);
			break ;
		case (3):
			normal = vec3(1, 0, 0);
			break ;
		case (4):
			normal = vec3(0, 1, 0);
			break ;
		case (5):
			normal = vec3(0, -1, 0);
			break ;
	}
	return (normal);
}

vec2	get_uv(int n, bool cw, float fall_size, bool floor)
{
	vec2	uv;
	int		base = 1;

	if (floor)
		base = 64;

	switch (n)
	{
		case (0):
			uv = vec2(base, fall_size);
			break ;
		case (1):
			uv = vec2(cw ? 0 : base, 0);
			break ;
		case (2):
			uv = vec2(cw ? base : 0, 0);
			break ;

		case (3):
			uv = vec2(base, fall_size);
			break ;
		case (4):
			uv = vec2(0, cw ? fall_size : 0);
			break ;
		case (5):
			uv = vec2(0, cw ? 0 : fall_size);
			break ;
	}

		
	return (uv);
}

void	main()
{
	vec4		pos;
	uint		normal_id, uv_id, block_type;
	float		fall_size;
	bool		floor;

	//////// Bitfield unpacking ///////////
	pos.x = data1 & 0xFFFF;
	pos.y = (data1 & 0xFFFF0000) >> 16u;
	pos.z = data2 & 0xFFFF;
	pos.w = 1.0f;

	uv_id		= ((uint(data2) & 0x70000u) >> 16u);
	fall_size	= float((uint(data2) & 0x1F80000u) >> 19u);
	normal_id	= (uint(data2) & 0xE000000u) >> 25u;
	block_type	= (uint(data2) & 0x70000000u) >> 28u;
	floor		= bool((uint(data2) & 0x80000000u) >> 31u);
	//////////////////////////////////////

	// Water ripples
	vType		= block_type;
	if (vType == 0) {
		float	time, wavelength;

		time		= u_time * 0.0001f;
		wavelength	= 0.05f;
		pos.y		+= (sin(pos.x * time) * cos(pos.y * time)) * wavelength + 0.5f;
	}

	// Output assignations
	vNormal		= get_normal(int(normal_id));
	vPosition	= vec3(pos);
	vTextCoord	= get_uv(int(uv_id), int(normal_id) == 2, fall_size, floor);

	vShadCoordP	= pos * dvp * dpp;
	vShadCoordS	= pos * dvs * dps;

	gl_Position	= pos * model * view * projection;
}
