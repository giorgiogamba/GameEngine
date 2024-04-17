// Copyright (c) 34BigThings, S.r.l. All rights reserved.

#pragma region Includes

#include <glm/glm.hpp>

#include "Public/Game.h"
#include "Public/Camera.h"
#include "Public/Shader.h"

#pragma endregion

#pragma region Lifecycle

Camera::Camera(const glm::vec3& Position, const glm::vec3& Direction, const glm::vec3& WorldUp)
{
	this->lightPosition = glm::vec3(0.f, 0.f, 2.f);

	this->Position = Position;
	this->WorldUpVector = WorldUp;
	this->FrontVector = glm::vec3(0.f, 0.f, -1.f); // Camera looks at negative Z axis
	this->RightVector = glm::vec3(1.f, 0.f, 0.f);
	this->UpVector = WorldUp;

	this->Roll = 0.f;
	this->Pitch = 0.f;
	this->Yaw = -90.f;

	this->Speed = 1.f;
	this->Sensitivity = 3.f;

	this->ProjectionMatrix = glm::mat4(1);
}

void Camera::Initialize(GLFWwindow* Window, Shader* Shader)
{
	if (!Window || !Shader)
		return;

	CreatePerspectiveMatrix(Window, Shader);
	Shader->AddUniformVector3fv(this->lightPosition, "lightPos");
}

void Camera::Update(GLFWwindow* Window, Shader* Shader, const float DeltaTime, const double MouseOffsetX, const double MouseOffsetY, Directions Direction)
{
	if (!Window || !Shader)
		return;

	UpdateMouseInput(DeltaTime, MouseOffsetX, MouseOffsetY);
	UpdateCameraVectors();
	UpdateKeyboardInput(DeltaTime, Direction);
	UpdateViewMatrix(Shader);
	Shader->AddUniformVector3fv(this->Position, "cameraPosition");
}

void Camera::UpdateMouseInput(const float DeltaTime, const double MouseOffsetX, const double MouseOffsetY)
{
	// Update pitch and constrant it in [-80, 80]
	this->Pitch += static_cast<GLfloat>(MouseOffsetY) * Sensitivity * DeltaTime;
	if (this->Pitch > 80.f)
	{
		this->Pitch = 80.f;
	}
	else if (this->Pitch < -80.f)
	{
		this->Pitch = -80.f;
	}

	this->Yaw += static_cast<GLfloat>(MouseOffsetX) * Sensitivity * DeltaTime;
	if (this->Yaw > 360.f || this->Yaw < -360.f)
	{
		this->Yaw = 0.f;
	}
}

void Camera::UpdateKeyboardInput(const float DeltaTime, const Directions Direction)
{
	switch (Direction)
	{
	case FORWARD:
		this->Position += this->FrontVector * Speed * DeltaTime;
		break;

	case BACKWARD:
		this->Position -= this->FrontVector * Speed * DeltaTime;
		break;

	case RIGHTWARD:
		this->Position += this->RightVector * Speed * DeltaTime;
		break;

	case LEFTWARD:
		this->Position -= this->RightVector * Speed * DeltaTime;
		break;

	case UPWARD:
		this->Position += this->UpVector * Speed * DeltaTime;
		break;

	case DOWNWARD:
		this->Position -= this->UpVector * Speed * DeltaTime;
		break; 

	default:
		break;
	}
}

void Camera::UpdateCameraVectors()
{
	this->FrontVector.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	this->FrontVector.y = sin(glm::radians(this->Pitch));
	this->FrontVector.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));

	this->FrontVector = glm::normalize(this->FrontVector);
	this->RightVector = glm::normalize(glm::cross(this->FrontVector, WorldUpVector));
	this->UpVector = glm::normalize(glm::cross(this->RightVector, this->FrontVector));
}

void Camera::CreatePerspectiveMatrix(GLFWwindow* Window, Shader* Shader)
{
	if (!Window || !Shader)
		return;

	int FrameBufferWidth = 0, FrameBufferHeight = 0;
	glfwGetFramebufferSize(Window, &FrameBufferWidth, &FrameBufferHeight);

	float fov = 90.f;
	float ZNearPlane = 0.1f;
	float  ZFarPlane = 500.f;
	this->ProjectionMatrix = glm::perspective
	(glm::radians(fov)
		, static_cast<float>(FrameBufferWidth) / FrameBufferHeight     // Aspect Ratio
		, ZNearPlane
		, ZFarPlane);

	Shader->AddUniformMatrix4fv(this->ProjectionMatrix, "ProjectionMatrix");
}

void Camera::UpdateViewMatrix(Shader* Shader)
{
	if (!Shader)
		return;

	const glm::mat4& ViewMatrix = glm::lookAt(this->Position, this->Position + this->FrontVector, this->UpVector);
	Shader->AddUniformMatrix4fv(ViewMatrix, "ViewMatrix");
}

#pragma endregion
