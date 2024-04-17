// Copyright Giorgio Gamba

#pragma once

#pragma region Types

enum Directions
{
	NONE = 0,
	FORWARD,
	BACKWARD,
	RIGHTWARD,
	LEFTWARD,
	UPWARD,
	DOWNWARD
};

#pragma endregion

class Shader;

class Camera
{

#pragma region Lifecycle

public:

	Camera(const glm::vec3& Position, const glm::vec3& Direction, const glm::vec3& WorldUp);

	void Initialize(GLFWwindow* Window, Shader* Shader);

	void Update(GLFWwindow* Window, Shader* Shader, const float DeltaTime, const double MouseOffsetX, const double MouseOffsetY, Directions Direction);

	void UpdateMouseInput(const float DeltaTime, const double MouseOffsetX, const double MouseOffsetY);

	void UpdateKeyboardInput(const float DeltaTime, const Directions Direction);
	
	void UpdateCameraVectors();

	void CreatePerspectiveMatrix(GLFWwindow* Window, Shader* Shader);

	void UpdateViewMatrix(Shader* Shader);

private:
	
	glm::vec3 lightPosition;

	glm::vec3 Position;
	glm::vec3 FrontVector;
	glm::vec3 WorldUpVector;
	glm::vec3 RightVector;
	glm::vec3 UpVector;

	float Pitch;
	float Roll;
	float Yaw;

	float Speed = 5.f;
	float Sensitivity = 10.f;

	glm::mat4 ProjectionMatrix;

#pragma endregion

};
