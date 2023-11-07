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
	vec3 direction = normalize(lightPos - vs_position);
	float diffuse = clamp (dot(direction, vs_normal), 0, 1);
	return material.diffuseColor * diffuse;
}

vec3 computeSpecularLightning()
{
	vec3 lightToPosDir = normalize(vec3(vs_position - lightPos));
	vec3 reflectLightToPosDir = reflect(lightToPosDir, vs_normal);
	vec3 posToViewDir = normalize(vec3(cameraPos - vs_position));
	float specularConstant = pow(max(dot(posToViewDir, reflectLightToPosDir), 0), 40);
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