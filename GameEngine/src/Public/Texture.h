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

class Texture
{
public:

#pragma region Lifecycle

    Texture(const char* texturePath);

	~Texture() {}

    void CreateTexture();

    void ApplyTexture(GLuint program);

    void unuse();

private:

    GLuint ID;

    const char* texturePath;

#pragma endregion

};