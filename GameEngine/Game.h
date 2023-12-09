// Copyright Giorgio Gamba

#pragma region Includes

#pragma once

#include <vector>

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

#pragma region Forward Declarations

class Shader;
class Material;
class Mesh;
class Texture;
class Camera;

#pragma endregion


class Game
{

#pragma region LifeCycle

public:

	Game();
	~Game();

private:

	void Init();

	void InitGameElements();

	void InitShaders();
	void InitTextures();
	void InitMaterials();
	void InitMeshes();
	void InitCamera();

#pragma endregion

#pragma region Window

public:

	GLFWwindow* GetWindow() const { return Window; }

private:

	void InitWindow(const char* WindowTitle, const int OpenGLMinVersion, const int OpenGLMajVersion, const bool bResizable);
	void SetupEngineVersion(const int OpenGLMinVersion, const int OpenGLMajVersion, const bool bResizable);
	GLFWwindow* CreateWindow(const char* WindowTitle);

	void SetupRendering();

	GLFWwindow* Window;

#pragma endregion

#pragma region Rendering

public:

	void Update();

	void UpdateMouseInput();

	void UpdateDeltaTime();

	void MaybeCloseWindow();

	void GetMovementDirection();

	void Render();

	void Reset();

	void AddMesh(Mesh* InMesh);

private:

	void ResetScreen();
	
	std::vector<Mesh*> Meshes;

	std::vector<Shader*> Shaders;

	std::vector<Material*> Materials;

	std::vector<Texture*> Textures;

	std::vector<Camera*> Cameras;

#pragma region MouseInput

	double LastMouseX;
	double LastMouseY;
	double MouseX;
	double MouseY;
	double MouseOffsetX;
	double MouseOffsetY;
	bool FirstMouse;

#pragma endregion

#pragma region Delta Time

	float DeltaTime;
	float CurrTime;
	float LastTime;

#pragma endregion

#pragma endregion
};
