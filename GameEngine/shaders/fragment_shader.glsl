#version 430

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord; 
in vec3 vs_normal;

out vec4 fs_color;

uniform sampler2D texture;

uniform vec3 lightPos;
uniform vec3 cameraPos;

void main()
{
	// Ambient Lightning
	vec3 ambientColor = vec3(0.1f, 0.1f, 0.1f);

	// Diffuse Lightning
	vec3 direction = normalize(vs_position - lightPos);
	vec3 diffuseColor = vec3(1.f, 1.f, 1.f);
	float diffuse = clamp (dot(direction, vs_normal), 0, 1);
	vec3 diffuseFinal = diffuseColor * diffuse;

	// Specular Lightning
	vec3 posToLightDir = normalize(vec3(lightPos - vs_position));
	vec3 reflectPosToLightDir = reflect(posToLightDir, vs_normal);
	vec3 posToViewDir = normalize(vec3(vs_position - cameraPos));
	float specularConstant = pow(max(dot(posToViewDir, reflectPosToLightDir), 0), 40);
	vec3 specularFinal = vec3(1.f, 1.f, 1.f) * specularConstant;


	fs_color = texture(texture, vs_texcoord) * vec4(vs_color, 1.f) * (vec4(ambientColor, 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f));
}