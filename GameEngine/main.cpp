// Copyright Giorgio Gamba

#pragma region Includes

#pragma region System Includes
#include <stdio.h>
#include <iostream>
#include <string>
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

#include "src/Public/Utils.h"

#pragma endregion
bool loadShaders(GLuint& program)
{
    char infoLog[512];
    GLint success;

    std::string tmp = "";
    std::string src = "";
    std::ifstream in_file;
    
    // Vertex shader creation
    in_file.open("shaders/vertex_shader.glsl");

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
        return false;
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
        return false;
    }


    // Fragment shader creation
    tmp = "";
    src = "";

    in_file.open("shaders/fragment_shader.glsl");

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
        return false;
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
        return false;
    }

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR: Unable to link program" << std::endl;
        std::cout << infoLog << std::endl;
        return false;
    }
    
    glUseProgram(0);
    glDeleteProgram(vertexShader);
    glDeleteProgram(fragmentShader);

    std::cout << "Program created correctly" << std::endl;
    return true;

}

void updateInput(GLFWwindow* window, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        position.z -= 0.005f;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        position.z += 0.005f;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        position.x -= 0.005f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        position.x += 0.005f;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        rotation.y -= 0.05f;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        rotation.y += 0.05f;
    }
}

GLuint CreateTexture()
{
    // TEXTURE 1
    // Texture Initialization
    int texwidth, textheight;
    unsigned char* teximage = SOIL_load_image("Textures/cat.png", &texwidth, &textheight, NULL, SOIL_LOAD_RGBA);

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

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
        return 0;
    }

    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(teximage);

    return tex;
}

void ApplyTexture(GLuint program, GLuint texture)
{
    glUniform1i(glGetUniformLocation(program, "texture"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
}

glm::mat4 CreateModelMatrix(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
{
    glm::mat4 modelMatrix(1.f); // Creates a diagonal identity matrix
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f)); // Rotation on the X axis
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f)); // Rotation on the Y axis
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f)); // Rotation on the Z axis
    modelMatrix = glm::scale(modelMatrix, scale);
    return modelMatrix;
}

glm::mat4 CreateViewMatrix()
{
    // View matrix definition
    glm::vec3 CameraUpVector(0.f, 1.f, 0.f); // generic up vector
    glm::vec3 CameraFrontVector(0.f, 0.f, -1.f); // Bu CGI definition, camera looks at negative Z axis
    glm::vec3 CameraPosition(0.f, 0.f, 2.f);
    glm::mat4 ViewMatrix(1.f);
    return glm::lookAt(CameraPosition, CameraPosition + CameraFrontVector, CameraUpVector);
}

glm::mat4 CreatePerspectiveMatrix(const int FrameBufferWidth, const int FrameBufferHeight)
{
    float fov = 90.f;
    float ZNearPlane = 0.1f;
    float  ZFarPlane = 500.f;
    glm::mat4 ProjectionMatrix(1.f);
    ProjectionMatrix = glm::perspective
    (glm::radians(fov)                                             // Field of view
        , static_cast<float>(FrameBufferWidth) / FrameBufferHeight     // Aspect Ratio
        , ZNearPlane
        , ZFarPlane);

    return ProjectionMatrix;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "Unable to initialize OpenGL screen" << std::endl;
        return -1;
    }

    // OpenGL Initialization
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game Engine", NULL, NULL);
    if (!window)
    {
        std::cout << "Unable to create OpenGL window" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error while initializing glew" << std::endl;
        return -1;
    }

    // Rendering settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Shaders creation
    GLuint program;
    if (!loadShaders(program))
    {
        std::cout << "Error while creating shaders" << std::endl;
        return -1;
    }

    // Buffers creation
    
    // Vertex Array
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Vertex Buffer
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Element Buffer
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position coordinates
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    // Rotation coordinates
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    // Texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
    glEnableVertexAttribArray(2);

    // Normal vector
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);

    glUniformMatrix4fv(glGetUniformLocation(program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(program, "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(program, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
    while (!glfwWindowShouldClose(window))
    {
        /* Poll for and process events */
        glfwPollEvents();
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glUseProgram(program);

        modelMatrix = CreateModelMatrix(position, rotation, scale);
        glUniformMatrix4fv(glGetUniformLocation(program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

        // Update window sizxe caching
        glfwGetFramebufferSize(window, &FrameBufferWidth, &FramebufferHeight);
        ProjectionMatrix = CreatePerspectiveMatrix(FrameBufferWidth, FramebufferHeight);
        glUniformMatrix4fv(glGetUniformLocation(program, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));


        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        glFlush();

        /* Poll for and process events */
        glfwPollEvents();
        glUseProgram(0);
    }

    glfwTerminate();
    return 0;
}