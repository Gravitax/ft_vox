#version 400 core

#define LIGHT_SOURCE_MAX	2

struct	LightSources {
	vec3	pos, dir, color;
	vec3	ambient, diffuse, specular;
	float	intensity;
};

struct	Light {
	bool	is_active, shadow;
	float	gamma;
};

in vec3					vNormal;
in vec3					vPosition;
in vec2					vTextCoord;
in vec4					vShadCoordP;
in vec4					vShadCoordS;
flat in float			vType;

uniform vec3			campos;
uniform float			u_time;

uniform Light			light;
uniform LightSources	light_sources[LIGHT_SOURCE_MAX];

uniform sampler2D		vTexture_0;
uniform sampler2D		vTexture_1;
uniform sampler2D		vTexture_2;
uniform sampler2D		vTexture_3;
uniform sampler2D		vTexture_4;
uniform sampler2D		vTexture_5;
uniform sampler2D		vTexture_6;
uniform sampler2D		vTexture_7;

uniform sampler2D		depthmap;

out vec4				FragColor;

// float	linearize_depth(float depth)
// {
// 	float	z;

// 	const float	near_plane	= 0.1f;
// 	const float	far_plane	= 50.0f;

// 	z	= depth * 2.0f - 1.0f; // Back to NDC
// 	z	= (2.0f * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
//     return (z / far_plane);
// }

float	percentage_closer_filtering(vec3 coords, float bias)
{
	float	depth, shadow;
	vec2	texel;

	texel	= 1.0f / textureSize(depthmap, 0);
	for (int x = -1; x <= 1; ++x) {
		for (int y = -1; y <= 1; ++y) {
			depth	= texture(depthmap, coords.xy + vec2(x, y) * texel).r;
			// depth	= linearize_depth(depth);
			shadow	+= depth + bias > coords.z ? 1.0f : 0.0f;        
		}    
	}
	shadow /= 10.0f;
	return (shadow);
}

float	compute_shadows(vec4 shadCoord, vec3 light_dir)
{
	float	bias;
	vec3	coords;

	coords	= shadCoord.xyz / shadCoord.w;
	// normalize coords
	coords	= coords * 0.5f + 0.5f;
	// cap shadows
	if (coords.z > 1.0f)
		coords.z = 0.0f;
	// calculate bias based on lightdir and normal
	bias	= max(0.001f * (1.0f - dot(vNormal, light_dir)), 0.0001f);
	// PCF EFFECT
	return (percentage_closer_filtering(coords, bias));
}

vec4	compute_light_sources(int i, LightSources source, vec3 color, vec3 view_dir)
{
	vec3	light_dir, reflect_dir;
	float	attenuation, e, shadows;

	color			*= source.color;

	light_dir		= normalize(-source.dir);
	reflect_dir		= reflect(light_dir, vNormal);

	source.ambient	= color * source.ambient;
	e				= max(dot(vNormal, light_dir), 0);
	source.diffuse	= color * source.diffuse * e;
	e				= pow(max(dot(view_dir, reflect_dir), 0), 8);
	source.specular	= color * source.specular * e;

	// shadows
	shadows			= light.shadow == true ? compute_shadows(i == 0 ? vShadCoordP : vShadCoordS, light_dir) : 1.0f;

	color 			= source.ambient + shadows * (source.diffuse + source.specular);

	// light attenuation
	e				= distance(source.pos, vPosition);
	attenuation		= min((1 / e) * source.intensity, 1.0f);
	color			*= attenuation;

	return (vec4(color, 1));
}

vec3	get_color(int index)
{
	vec3	color;

	switch (index) {
		case (0):
			color = texture(vTexture_0, vTextCoord).rgb;
			break ;
		case (1):
			color = texture(vTexture_1, vTextCoord).rgb;
			break ;
		case (2):
			color = texture(vTexture_2, vTextCoord).rgb;
			break ;
		case (3):
			color = texture(vTexture_3, vTextCoord).rgb;
			break ;
		case (4):
			color = texture(vTexture_4, vTextCoord).rgb;
			break ;
		case (5):
			color = texture(vTexture_5, vTextCoord).rgb;
			break ;
		case (6):
			color = texture(vTexture_6, vTextCoord).rgb;
			break ;
		case (7):
			color = texture(vTexture_7, vTextCoord).rgb;
			break ;
	}
	return (color);
}

void	main()
{
	int		index	= int(vType);
	vec3	color	= get_color(index);

	if (light.is_active == true) {
		vec3		view_dir	= normalize(campos - vPosition);
		int			i			= -1;

		FragColor = vec4(0);
		while (++i < LIGHT_SOURCE_MAX)
			FragColor += compute_light_sources(i, light_sources[i], color, view_dir);
	} else {
		FragColor = vec4(color, 1);
	}

	// water transparency
	if (index == 0)
		FragColor.a = 0.5f;
	// gamma correction
	FragColor.rgb = pow(FragColor.rgb, vec3(1 / light.gamma));
}
