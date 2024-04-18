// Copyright Giorgio Gamba

#pragma once

#include "Shader.h"

class Material
{
public:

	Material(glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor, Shader* shader)
	{
		if (!shader)
			return;

		this->ambientColor = ambientColor;
		this->diffuseColor = diffuseColor;
		this->specularColor = specularColor;

		ApplyToShader(shader);
	}

	~Material() {}

	void ApplyToShader(Shader* shader);
	
private:

	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;

};