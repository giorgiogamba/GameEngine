#version 430

struct Material
{
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
};

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord; 
in vec3 vs_normal;

out vec4 fs_color;

uniform sampler2D texture;
uniform Material material;

uniform vec3 lightPos;
uniform vec3 cameraPos;

vec3 computeDiffuseLightning()
{
	vec3 direction = normalize(vs_position - lightPos);
	float diffuse = clamp (dot(direction, vs_normal), 0, 1);
	return material.diffuseColor * diffuse;
}

vec3 computeSpecularLightning()
{
	vec3 posToLightDir = normalize(vec3(lightPos - vs_position));
	vec3 reflectPosToLightDir = reflect(posToLightDir, vs_normal);
	vec3 posToViewDir = normalize(vec3(vs_position - cameraPos));
	float specularConstant = pow(max(dot(posToViewDir, reflectPosToLightDir), 0), 40);
	return vec3(1.f, 1.f, 1.f) * specularConstant;
}

void main()
{
	fs_color = texture(texture, vs_texcoord)
			 * vec4(vs_color, 1.f)
			 * (vec4(material.ambientColor, 1.f)
				+ vec4(computeDiffuseLightning(), 1.f)
				+ vec4(computeSpecularLightning(), 1.f));
}