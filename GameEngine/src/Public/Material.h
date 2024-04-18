// Copyright Giorgio Gamba

#pragma region Includes

#pragma once

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

#include "Shader.h"

#pragma endregion

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