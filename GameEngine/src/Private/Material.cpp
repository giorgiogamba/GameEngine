// Copyright Giorgio Gamba

#include "Material.h"

void Material::ApplyToShader(Shader* shader)
{
	if (!shader)
		return;

	shader->AddUniformVector3fv(ambientColor, "material.ambientColor");
	shader->AddUniformVector3fv(diffuseColor, "material.diffuseColor");
	shader->AddUniformVector3fv(specularColor, "material.specularColor");
}

