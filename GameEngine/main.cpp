// Copyright Giorgio Gamba

#pragma region Includes

#pragma once

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

#pragma region Local Includes

#include "src/Public/Utils.h"
#include "src/Public/Shader.h"
#include "src/Public/Texture.h"
#include "src/Public/Material.h"
#include "src/Public/Camera.h"
#include "src/Public/Mesh.h"

#pragma endregion

#pragma endregion

void SetupRendering()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void glDebugOutput(GLenum source,
    GLenum type,
    unsigned int id,
    GLenum severity,
    GLsizei length,
    const char* message,
    const void* userParam)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
    case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}

void SetupEngineVersion()
{
    
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
}

GLFWwindow* CreateWindow()
{
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game Engine", NULL, NULL);
    if (!window)
    {
        std::cout << "Unable to create OpenGL window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    return window;
}

GLFWwindow* SetupEngine()
{
    GLFWwindow* window = nullptr;

    if (!glfwInit())
    {
        std::cout << "Unable to initialize OpenGL screen" << std::endl;
        return nullptr;
    }

    window = CreateWindow();
    if (!window)
        return nullptr;


    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error while initializing glew" << std::endl;
        return nullptr;
    }

    return window;
}

void ResetScreen()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

int main(void)
{
    SetupEngineVersion(); 
    GLFWwindow* window = SetupEngine();
    if (!window)
    {
        std::cout << "Unable to create the window" << std::endl;
        return -1;
    }
    
    SetupRendering();

    /*glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);*/

    Shader shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");
    Material material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f));
    material.ApplyToShader(&shader);
    
    // Vertex Array
    Mesh mesh;
    Texture texture("Textures/cat.png");
    
    Camera camera;
    camera.AddToShader(&shader);
    camera.UpdatePerspectiveMatrix(window, &shader);
    camera.CreateViewMatrix(&shader);

    // Rendering loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        camera.UpdatePerspectiveMatrix(window, &shader);
        mesh.Update(window, &shader);

        ResetScreen();

        // Render
        shader.use();
        texture.ApplyTexture(shader.GetID());

        mesh.Draw();

        glfwSwapBuffers(window);
        glFlush();

        // Reset state
        glBindVertexArray(0);
        shader.unuse();
        texture.unuse();
    }

    glfwTerminate();
    return 0;
}