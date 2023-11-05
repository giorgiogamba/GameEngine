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

class Shader
{
public:

	Shader(const char* InVertexShaderPath, const char* InFragmentShaderPath)
    {
        ID = -1;
        vertexShaderPath = InVertexShaderPath;
        fragmentShaderPath = InFragmentShaderPath;

        loadShaders();
    }

	~Shader()
    {
        glDeleteProgram(ID);
    }

    void loadShaders()
    {
        char infoLog[512];
        GLint success;

        std::string tmp = "";
        std::string src = "";
        std::ifstream in_file;

        // Vertex shader creation
        in_file.open(vertexShaderPath);

        if (in_file.is_open())
        {
            while (std::getline(in_file, tmp))
            {
                src += tmp + "\n";
            }
        }
        else
        {
            std::cout << "Error while opening vertex shader file" << std::endl;
            return;
        }

        in_file.close();

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const GLchar* vertexShaderSrc = src.c_str();
        glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
        glCompileShader(vertexShader);

        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR: Could not compile vertex shader" << std::endl;
            std::cout << infoLog << std::endl;
            return;
        }

        // Fragment shader creation
        tmp = "";
        src = "";

        in_file.open(fragmentShaderPath);

        if (in_file.is_open())
        {
            while (std::getline(in_file, tmp))
            {
                src += tmp + "\n";
            }
        }
        else
        {
            std::cout << "Error while opening fragment shader file" << std::endl;
            return;
        }

        in_file.close();

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const GLchar* fragmentShaderSrc = src.c_str();
        glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR: Could not compile fragment shader" << std::endl;
            std::cout << infoLog << std::endl;
            return;
        }

        ID = glCreateProgram();
        glAttachShader(ID, vertexShader);
        glAttachShader(ID, fragmentShader);
        glLinkProgram(ID);

        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "ERROR: Unable to link program" << std::endl;
            std::cout << infoLog << std::endl;
            return;
        }

        glUseProgram(0);
        glDeleteProgram(vertexShader);
        glDeleteProgram(fragmentShader);

        std::cout << "Program created correctly" << std::endl;
    }

    void AddUniformMatrix4fv(glm::mat4 matrix, const GLchar* matrixName)
    {
        use();
        glUniformMatrix4fv(glGetUniformLocation(ID, matrixName), 1, GL_FALSE, glm::value_ptr(matrix));
        unuse();
    }

    void AddUniformVector3fv(glm::vec3 vector, const GLchar* vectorName)
    {
        use();
        glUniform3fv(glGetUniformLocation(ID, vectorName), 1, glm::value_ptr(vector));
        unuse();
    }

    void DrawTriangles(GLuint vertexArray, int numIndices)
    {
        glBindVertexArray(vertexArray);
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    }

    void use()
    {
        glUseProgram(ID);
    }

    void unuse()
    {
        glUseProgram(0);
    }

    GLuint GetID() { return ID; }

private:

	GLuint ID;

    const char* vertexShaderPath;
    const char* fragmentShaderPath;
};