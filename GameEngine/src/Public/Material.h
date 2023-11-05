// Copyright Giorgio Gamba

#pragma region Includes

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/common.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/GL.h>
#include <gl/GLU.h>
#include <SOIL2/SOIL2.h>

#pragma endregion

class Material
{
public:

	Material(glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor)
	{
		this->ambientColor = ambientColor;
		this->diffuseColor = diffuseColor;
		this->specularColor = specularColor;
	}

	void ApplyToShader(Shader* shader)
	{
		shader->AddUniformVector3fv(ambientColor, "material.ambientColor");
		shader->AddUniformVector3fv(diffuseColor, "material.diffuseColor");
		shader->AddUniformVector3fv(specularColor, "material.specularColor");
	}

	~Material() {}

private:

	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;

};