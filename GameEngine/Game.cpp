// Copyright Giorgio Gamba

#pragma region Includes

#pragma once

#include "Game.h"

#include <iostream>
#include <GL/glew.h>

#include "src/Config/Configuration.h"
#include "src/Public/Shader.h"
#include "src/Public/Material.h"
#include "src/Public/Primitive.h"
#include "src/Public/Mesh.h"
#include "src/Public/Texture.h"
#include "src/Public/Camera.h"

#pragma endregion

#pragma region LifeCycle

Game::Game()
{
	Init();
}

Game::~Game()
{
    glfwDestroyWindow(Window);
    glfwTerminate();

    // Delete all the structures
    for (std::vector<Mesh*>::iterator it = Meshes.begin(); it < Meshes.end(); it++)
    {
        if (!*it)
            continue;

        delete* it;
    }

    for (std::vector<Shader*>::iterator it = Shaders.begin(); it < Shaders.end(); it++)
    {
        if (!*it)
            continue;

        delete* it;
    }

    for (std::vector<Texture*>::iterator it = Textures.begin(); it < Textures.end(); it++)
    {
        if (!*it)
            continue;

        delete* it;
    }

    for (std::vector<Material*>::iterator it = Materials.begin(); it < Materials.end(); it++)
    {
        if (!*it)
            continue;

        delete* it;
    }
}

void Game::Init()
{
    InitWindow(WINDOWNAME, OPENGLMINVERSION, OPENGLMAJVERSION, RESIZABLE);
    InitGameElements();
}

void Game::InitGameElements()
{
    InitShaders();
    InitMeshes();
    InitMaterials();
    InitTextures();
    InitCamera();
}

void Game::InitCamera()
{
    Shader* CurrShader = Shaders[0];
    if (!CurrShader || !Window)
        return;

    Camera NewCamera;
    Cameras.push_back(&NewCamera);

    if (Cameras.size() == 0)
        return;

    Cameras[0]->AddToShader(CurrShader);
    Cameras[0]->UpdatePerspectiveMatrix(Window, CurrShader);
    Cameras[0]->CreateViewMatrix(CurrShader);
}

void Game::InitShaders()
{
    this->Shaders.push_back(new Shader(VERTEXSHADERPATH, FRAGMENTSHADERPATH));
}

void Game::InitTextures()
{
    this->Textures.push_back(new Texture("Textures/cat.png"));
}

void Game::InitMaterials()
{
    if (!Shaders[0])
        return;

    this->Materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), Shaders[0]));
}

void Game::InitMeshes()
{
    Meshes.push_back(new Mesh(new Quad()));
}

#pragma endregion

#pragma region Window

void Game::InitWindow(const char* WindowTitle, const int OpenGLMinVersion, const int OpenGLMajVersion, const bool bResizable)
{
    SetupEngineVersion(OpenGLMinVersion, OpenGLMajVersion, bResizable);

    if (!glfwInit())
    {
        std::cout << "Error while initializing GLFW" << std::endl;
        return;
    }

    Window = CreateWindow(WindowTitle);
    if (!Window)
        return;

    glfwMakeContextCurrent(Window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error while initializing glew" << std::endl;
        return;
    }

    SetupRendering();
}

void Game::SetupEngineVersion(const int OpenGLMinVersion, const int OpenGLMajVersion, const bool bResizable)
{
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OpenGLMajVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OpenGLMinVersion);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, bResizable);
}

GLFWwindow* Game::CreateWindow(const char* WindowTitle)
{
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WindowTitle, NULL, NULL);
    if (!window)
    {
        std::cout << "Unable to create OpenGL window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    return window;
}

void Game::SetupRendering()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

#pragma endregion

#pragma region Render

void Game::Update()
{
    if (!Window)
        return;

    for (Mesh* Mesh : Meshes)
    {
        Shader* CurrShader = Shaders[0];
        if (!CurrShader || !Mesh)
            continue;

        Mesh->Update(Window, Shaders[0]);
    }

    glfwPollEvents();

    for (Shader* Shader : Shaders)
    {
        if (!Shader)
            continue;

        Cameras[0]->UpdatePerspectiveMatrix(Window, Shader);
    }
}

void Game::Render()
{
    ResetScreen();

    for (Texture* Texture : Textures)
    {
        Shader* CurrShader = Shaders[0];
        if (!CurrShader || !Texture)
            continue;

        CurrShader->use();
        Texture->ApplyTexture(Shaders[0]->GetID());
    }

    for (Mesh* Mesh : Meshes)
    {
        if (!Mesh)
            continue;

        Mesh->Draw();
    }

    glfwSwapBuffers(Window);
    glFlush();
}

void Game::Reset()
{
    glBindVertexArray(0);

    for (Shader* Shader : Shaders)
    {
        if (!Shader)
            continue;

        Shader->unuse();
    }

    for (Texture* Texture : Textures)
    {
        if (!Texture)
            continue;

        Texture->unuse();
    }
}

void Game::ResetScreen()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Game::AddMesh(Mesh* InMesh)
{
    Meshes.push_back(InMesh);
}

#pragma endregion
