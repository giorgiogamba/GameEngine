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

#pragma region Local Includes

#include "src/Public/Utils.h"
#include "src/Public/Shader.h"
#include "src/Public/Texture.h"
#include "src/Public/Material.h"

#pragma endregion

#pragma endregion

#pragma region Triangle Vertices

// Basic 2 triangles definition
Vertex vertices[] =
{
    glm::vec3(-0.5f, 0.5f, 0.0f),       glm::vec3(1.0f, 0.0f, 0.0f),    glm::vec2(0.0f, 1.0f), glm::vec3(0.f, 0.f, -1.f),
    glm::vec3(-0.5f, -0.5f, 0.0f),      glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(0.0f, 0.0f), glm::vec3(0.f, 0.f, -1.f),
    glm::vec3(0.5f, -0.5f, 0.0f),       glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(1.0f, 0.0f), glm::vec3(0.f, 0.f, -1.f),
    glm::vec3(0.5f, 0.5f, 0.0f),        glm::vec3(1.0f, 1.0f, 0.0f),    glm::vec2(1.0f, 1.0f), glm::vec3(0.f, 0.f, -1.f),
};

unsigned numVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indices[] =
{
    0, 1, 2,
    0, 2, 3
};

unsigned numIndices = 6;

#pragma endregion

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

glm::mat4 CreatePerspectiveMatrix(GLFWwindow* window)
{
    int FrameBufferWidth = 0, FrameBufferHeight = 0;
    glfwGetFramebufferSize(window, &FrameBufferWidth, &FrameBufferHeight);

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

void EnableVertexPointer()
{
    int PointerIndex = 0;

    // Position coordinates
    glVertexAttribPointer(PointerIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
    glEnableVertexAttribArray(PointerIndex);

    // Rotation coordinates
    PointerIndex++;
    glVertexAttribPointer(PointerIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
    glEnableVertexAttribArray(PointerIndex);

    // Texture coordinates
    PointerIndex++;
    glVertexAttribPointer(PointerIndex, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
    glEnableVertexAttribArray(PointerIndex);

    // Normal vector
    PointerIndex++;
    glVertexAttribPointer(PointerIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(PointerIndex);
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

    EnableVertexPointer();

    glBindVertexArray(0);

    Texture texture("Textures/cat.png");

    // Model matrix creation
    // Collects all the transform informations for the given object
    // Remember that matrices multiply from right to left

    glm::vec3 position(0.f), rotation(0.f), scale(1.f);
    glm::mat4 modelMatrix = CreateModelMatrix(position, rotation, scale);
    glm::mat4 ViewMatrix = CreateViewMatrix();
    glm::mat4 ProjectionMatrix = CreatePerspectiveMatrix(window);

    shader.AddUniformMatrix4fv(modelMatrix, "ModelMatrix");
    shader.AddUniformMatrix4fv(ViewMatrix, "ViewMatrix");
    shader.AddUniformMatrix4fv(ProjectionMatrix, "ProjectionMatrix");

    glm::vec3 LightsPosition(0.f, 0.f, 2.f), CameraPosition(0.f, 0.f, 2.f);
    shader.AddUniformVector3fv(LightsPosition, "lightPos");
    shader.AddUniformVector3fv(CameraPosition, "cameraPos");

    // Rendering loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        updateInput(window, position, rotation, scale);

        // Update matrices
        modelMatrix = CreateModelMatrix(position, rotation, scale);
        shader.AddUniformMatrix4fv(modelMatrix, "ModelMatrix");

        ProjectionMatrix = CreatePerspectiveMatrix(window);
        shader.AddUniformMatrix4fv(ProjectionMatrix, "ProjectionMatrix");

        // Clean screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // Render
        shader.use();
        texture.ApplyTexture(shader.GetID());

        shader.DrawTriangles(VAO, numIndices);

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