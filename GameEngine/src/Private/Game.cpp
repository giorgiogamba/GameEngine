// Copyright Giorgio Gamba

#pragma region Includes

#pragma once

#include "Public/Game.h"

#include <iostream>
#include <GL/glew.h>

#include "Config/Configuration.h"
#include "Public/Shader.h"
#include "Public/Material.h"
#include "Public/Primitive.h"
#include "Public/Mesh.h"
#include "Public/Model.h"
#include "Public/Texture.h"
#include "Public/Camera.h"
#include "Public/ObjectLoader.h"

#pragma endregion

#pragma region LifeCycle

Game::Game()
{
    this->DeltaTime = 0;
    this->CurrTime = 0;
    this->LastTime = 0;

    this->LastMouseX = 0.0;
    this->LastMouseY = 0.0;
    this->MouseX = 0.0;
    this->MouseY = 0.0;
    this->MouseOffsetX = 0.0;
    this->MouseOffsetY = 0.0;
    this->FirstMouse = true;

	Init();
}

Game::~Game()
{
    glfwDestroyWindow(Window);
    glfwTerminate();

    // Delete all the structures
    for (std::vector<Model*>::iterator it = Models.begin(); it < Models.end(); it++)
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
    InitMaterials();
    InitTextures();
    InitCamera();
    InitModels();
}

void Game::InitCamera()
{
    Shader* CurrShader = Shaders[0];
    if (!CurrShader || !Window)
        return;

    Cameras.push_back(new Camera(glm::vec3(0.f, 0.f, 2.f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f)));
    Cameras[0]->Initialize(Window, CurrShader);
}

void Game::InitShaders()
{
    this->Shaders.push_back(new Shader(VERTEXSHADERPATH, FRAGMENTSHADERPATH));
}

void Game::InitTextures()
{
    this->Textures.push_back(new Texture("Textures/cat.png"));
    this->Textures.push_back(new Texture("Textures/tex1.jpg"));
}

void Game::InitMaterials()
{
    if (!Shaders[0])
        return;

    this->Materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), Shaders[0]));
}

void Game::InitModels()
{
    
    std::vector<Vertex> Cube = LoadObject("obj_files/cube.obj");
    //Meshes.push_back(new Mesh(Cube.data(), Cube.size(), NULL, 0/*, glm::vec3(1.f, 0.f, 0.f)*/, glm::vec3(0.f), glm::vec3(0.f), glm::vec3(1.f)));

    std::vector<Mesh*> Meshes2;
    Meshes2.push_back(new Mesh(new Pyramid()));
    Models.push_back(new Model("Pyr1", glm::vec3(0.f, 0.f, 0.f), Materials[0], Textures[0], Meshes2));
    
    std::vector<Mesh*> Meshes;
    Meshes.push_back(new Mesh(new Quad()));
    Models.push_back(new Model("CubeObject", glm::vec3(1.f, 0.f, 0.f), Materials[0], Textures[1], Meshes));

	std::vector<Mesh*> Meshes3;
	Meshes3.push_back(new Mesh(new Triangle()));
	Models.push_back(new Model("CubeObject", glm::vec3(-1.f, 0.f, 0.f), Materials[0], Textures[0], Meshes3));

    std::vector<Mesh*> FloorMeshes;
    FloorMeshes.push_back(new Mesh(new Quad()));
    Models.push_back(new Model("Floor", glm::vec3(0.f, 0.f, 0.f), Materials[0], Textures[0], FloorMeshes));
    Models[3]->Move(glm::vec3(0.f, -0.5f, 0.f));
    Models[3]->Scale(glm::vec3(10.f, 10.f, 0.f));
    Models[3]->Rotate(glm::vec3(-45.f, 0.f, 0.f));

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

    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

#pragma endregion

#pragma region Render

void Game::Update()
{
    if (!Window)
        return;

    glfwPollEvents();

    MaybeCloseWindow();
    UpdateDeltaTime();
    //UpdateMouseInput();
    GetMovementDirection();
    glFlush();
}

void Game::UpdateDeltaTime()
{
    CurrTime = static_cast<float>(glfwGetTime()); // Evaluate if use static cast
    DeltaTime = CurrTime - LastTime;
    LastTime = static_cast<float>(CurrTime);

    //std::cout << "Delta Time " << DeltaTime << std::endl;
}

void Game::UpdateMouseInput()
{
    glfwGetCursorPos(Window, &this->MouseX, &this->MouseY);
    if (this->FirstMouse) // Initialization
    {
        this->LastMouseX = this->MouseX;
        this->LastMouseY = this->MouseY;
        this->FirstMouse = false;
    }

    this->MouseOffsetX = this->MouseX - this->LastMouseX;
    this->MouseOffsetY = this->MouseY - this->LastMouseY;

    this->LastMouseX = this->MouseX;
    this->LastMouseY = this->MouseY;
}

void Game::Render()
{
    ResetScreen();

    for (Model* Model : Models)
    {
        if (!Model)
            continue;

        Model->Update(Window, Shaders[0]);
        Model->Render(Shaders[0]);
    }

    glfwSwapBuffers(Window);
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

    // If active, removes multiple textures instancing
    /*for (Texture* Texture : Textures)
    {
        if (!Texture)
            continue;

        Texture->unuse();
    }*/
}

void Game::ResetScreen()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Game::GetMovementDirection()
{
    Directions CurrDirection = NONE;

    if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
    {
        CurrDirection = FORWARD;
    }
    if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
    {
        CurrDirection = BACKWARD;
    }
    if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
    {
        CurrDirection = RIGHTWARD;
    }
    if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
    {
        CurrDirection = LEFTWARD;
    }
    if (glfwGetKey(Window, GLFW_KEY_C) == GLFW_PRESS)
    {
        CurrDirection = UPWARD;
    }
    if (glfwGetKey(Window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        CurrDirection = DOWNWARD;
    }

    Cameras[0]->Update(Window, Shaders[0], DeltaTime, MouseOffsetX, MouseOffsetY, CurrDirection);
}

void Game::MaybeCloseWindow()
{
    if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(Window, GLFW_TRUE);
    }
}

#pragma endregion
