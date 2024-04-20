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
#include "Model.h"
#include "src/Public/Texture.h"
#include "src/Public/Camera.h"
#include "ObjectLoader.h"
#include "Light.h"
#include "PhysicsEngine.h"

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

    for (std::vector<PointLight*>::iterator it = PointLights.begin(); it < PointLights.end(); it++)
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
    InitSkybox();
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
    this->Textures.push_back(new Texture("Textures/white.jpg"));
    this->Textures.push_back(new Texture("Textures/sky.jpg"));
}
    
void Game::InitMaterials()
{
    if (!Shaders[0])
        return;

    this->Materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), Shaders[0]));
}

void Game::InitModels()
{
    // Environment models
    std::vector<Mesh*> EnvMeshes;
    Mesh* TerrainMesh = new Mesh(new Quad());
    TerrainMesh->Move(glm::vec3(0.f, -1.f, 0.f));
    TerrainMesh->Rotate(glm::vec3(-90.f, 0.f, 0.f));
    TerrainMesh->Scale(glm::vec3(10.f, 10.f, 0.f));
    EnvMeshes.push_back(TerrainMesh);
    Models.push_back(new Model("Terrain", glm::vec3(0.f, 0.f, 0.f), Materials[0], Textures[0], EnvMeshes));

    // Models
    std::vector<Mesh*> Meshes;
    Meshes.push_back(new Mesh(new Pyramid()));
    Models.push_back(new Model("Pyr1", glm::vec3(0.f, 0.f, 0.f), Materials[0], Textures[0], Meshes));

    std::vector<Mesh*> Meshes2;
    Meshes2.push_back(new Mesh(new Pyramid()));
    Meshes2[0]->Move(glm::vec3(-3.f, 0.f, 0.f));
    Models.push_back(new Model("Pyr2", glm::vec3(0.f, 0.f, 0.f), Materials[0], Textures[0], Meshes2));

    std::vector<Mesh*> Meshes3;
    Meshes3.push_back(new Mesh(new Pyramid()));
    Meshes3[0]->Move(glm::vec3(3.f, 0.f, 0.f));
    Models.push_back(new Model("Pyr3", glm::vec3(0.f, 0.f, 0.f), Materials[0], Textures[0], Meshes3));

    std::vector<Mesh*> Meshes4;
    Meshes4.push_back(new Mesh(new Cube()));
    Meshes4[0]->Move(glm::vec3(1.5f, 0.f, 0.f));
    Models.push_back(new Model("Cube", glm::vec3(0.f, 0.f, 0.f), Materials[0], Textures[0], Meshes4));

    //std::vector<Mesh*> CubeMeshes;
    //Mesh CubeMesh = Mesh(new Cube());
    //CubeMeshes.push_back(&CubeMesh);
    //Models.push_back(new Model("PhysicsCube", glm::vec3(0.f, 0.f, 0.f), Materials[0], Textures[0], CubeMeshes));

    //// Cube Physics model
    //RigidBody CubeRigidBody;
    //CubeRigidBody.Mesh = &CubeMesh;
    //CubeRigidBody.X = glm::vec3(0.0f, 1.5f, 0.0f);
    //CubeRigidBody.V = glm::vec3(1.0f, 0.0f, 1.0f);

}

void Game::InitLight()
{
    InitPointLights();
}

void Game::InitPointLights()
{
    PointLights.push_back(new PointLight(glm::vec3(0.f)));
    //UpdatePointLights();
}

void Game::InitSkybox()
{
    // Create a cube with a big scale and set its position the same as the camera
    std::vector<Mesh*> Skybox;
    Skybox.push_back(new Mesh(new Cube()));
    SkyboxModel = new Model("Skybox", glm::vec3(0.f, 0.f, 0.f), Materials[0], Textures[1], Skybox);
    //SkyboxModel->Scale(glm::vec3(10.f, 20.f, 0.f));
}

void Game::UpdatePointLights()
{
    if (!Shaders.empty())
    {
        for (PointLight* PointLight : PointLights)
        {
            if (!PointLight)
                continue;

            PointLight->SendToShader(Shaders[0]);
        }
    }
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
    UpdateMouseInput();
    GetMovementDirection();

    /*The position of the center of mass of a body is computed through the First ORder Euler Integration,
    which basically discretizes an integral with finite steps*/


    // Update - This is basically a step simulation
    // First version: directly depends on the delta time, which is the rendering time -> BAD
    // CubeRigidBody.X += /*DeltaTime * */CubeRigidBody.V; 

    // Second version: depends on a constant, in some way connected to the delta time
    const float DeltaTimeLimit = 0.001f;

    // Computation of the physics time, recorves all the overlapped time stamps is missed
    TimeAccuracy += DeltaTime;
    while (TimeAccuracy < DeltaTimeLimit)
    {
        //CubeRigidBody.X += TimeAccuracy * CubeRigidBody.V; // Velocit Update

        // Rotation matrix update
        /*Since there is not the same relationship between velocity and position (velocity is the derivative of the position)
        while the rotation matrix is not the derivative of the angular momentum, it is necessary to find a ne relationship to connect them.
        Supposed a point in the body rotates around Omega (which is the rotation axis in fact), it describes a circle around the axis.
        At every time step we should find the tangent of the circle on that point, which is in fact the rotation matrix.
        We note that the rotation matrix is perpendicular to the point, while the point itself is perpendicular to the 
        omeega, so the rotatio matrix is perpendicular to both of them -> VECTOR CROSS PRODUCT fi we apply
        the same operation cross product between omega and the 3 axis of the body*/

        // #TODO Compute cross product
        //CubeRigidBody.R += TimeAccuracy /** Cross(CubeRigidBody.Omega)*/ * CubeRigidBody->R;

        TimeAccuracy -= DeltaTimeLimit;
    }


    UpdatePointLights();
    glFlush();

    ResetScreen();


    for (Texture* Texture : Textures)
    {
        Shader* CurrShader = Shaders[0];
        if (!CurrShader || !Texture)
            continue;

        CurrShader->use();
        Texture->ApplyTexture(Shaders[0]->GetID());
    }

    // Render skydome first
    glFrontFace(GL_CCW);
    glDisable(GL_DEPTH_TEST);
    SkyboxModel->Update(Window, Shaders[0]);
    SkyboxModel->Render(Shaders[0]);
    glEnable(GL_DEPTH_TEST);

    for (Model* Model : Models)
    {
        if (!Model)
            continue;

        Model->Update(Window, Shaders[0]);
        Model->Render(Shaders[0]);
    }
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
    /*ResetScreen();*/

    /*for (Texture* Texture : Textures)
    {
        Shader* CurrShader = Shaders[0];
        if (!CurrShader || !Texture)
            continue;

        CurrShader->use();
        Texture->ApplyTexture(Shaders[0]->GetID());
    }*/

    /*for (Model* Model : Models)
    {
        if (!Model)
            continue;

        Model->Render(Shaders[0]);
    }*/

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
    if (glfwGetKey(Window, GLFW_KEY_1) == GLFW_PRESS)
    {
        for (PointLight* PointLight : PointLights)
        { 
            if (!PointLight || !Cameras[0])
                continue;

            PointLight->SetPosition(Cameras[0]->GetPosition());
        }
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
