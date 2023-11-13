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

	Texture(const char* texturePath)
    {
        this->texturePath = texturePath;
        CreateTexture();
    }

	~Texture() {}

    void CreateTexture()
    {
        int texwidth, textheight;
        unsigned char* teximage = SOIL_load_image(this->texturePath, &texwidth, &textheight, NULL, SOIL_LOAD_RGBA);

        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        if (teximage)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texwidth, textheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, teximage);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "ERROR: error while loading texture" << std::endl;
            return;
        }

        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        SOIL_free_image_data(teximage);
    }

    void ApplyTexture(GLuint program)
    {
        glUniform1i(glGetUniformLocation(program, "texture"), 0);
        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, ID);
    }

    void unuse()
    {
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

private:

    GLuint ID;

    const char* texturePath;

};