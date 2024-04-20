#version 430

struct Material
{
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
};

struct PointLight
{
	vec3 Position;
	float Intensity;
	vec3 Color;
	float Constant;
	float Linear;
	float Quadratic;
};

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord; 
in vec3 vs_normal;

out vec4 fs_color;

uniform sampler2D texture;
uniform Material material;
uniform PointLight Light;
uniform vec3 cameraPosition;

vec3 computeDiffuseLightning()
{
	vec3 direction = normalize(Light.Position - vs_position);
	float diffuse = clamp (dot(direction, vs_normal), 0, 1);
	return material.diffuseColor * diffuse;
}

vec3 computeSpecularLightning()
{
	vec3 lightToPosDir = normalize(vs_position - Light.Position);
	vec3 reflectLightToPosDir = normalize(reflect(lightToPosDir, normalize(vs_normal)));
	vec3 posToViewDir = normalize(vec3(cameraPosition - vs_position));
	float specularConstant = pow(max(dot(posToViewDir, reflectLightToPosDir), 0), 35);
	return material.specularColor * specularConstant;
}

float computeAttenuation()
{
	float distance = length(Light.Position - vs_position);
	float attenuation = 1.f / (Light.Constant + Light.Linear * distance + Light.Quadratic * distance * distance * 10);
	return attenuation;
}

void main()
{
	fs_color = texture(texture, vs_texcoord)
			 // * vec4(vs_color, 1.f)
			 * (vec4(material.ambientColor, 1.f) + vec4(computeDiffuseLightning(), 1.f) + vec4(computeSpecularLightning(), 1.f));
			 //* computeAttenuation();
}