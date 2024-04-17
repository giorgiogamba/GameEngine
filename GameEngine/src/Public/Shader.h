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

#pragma endregion

class Shader
{

#pragma region Lifecycle

public:

    Shader(const char* InVertexShaderPath, const char* InFragmentShaderPath);

    ~Shader();

	void loadShaders();

private:

	const char* vertexShaderPath;

	const char* fragmentShaderPath;

#pragma endregion

#pragma region Render

public:

	void AddUniformMatrix4fv(glm::mat4 matrix, const GLchar* matrixName);

	void AddUniformVector3fv(glm::vec3 vector, const GLchar* vectorName);

    void use() { glUseProgram(ID); }

    void unuse() { glUseProgram(0); }

    GLuint GetID() { return ID; }

private:

	GLuint ID;

#pragma endregion

};
