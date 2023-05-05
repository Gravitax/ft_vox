#version 400 core

in vec3					vPosition;

uniform samplerCube		vSkybox;

out vec4				FragColor;

void	main()
{
	FragColor = texture(vSkybox, vPosition);
}
