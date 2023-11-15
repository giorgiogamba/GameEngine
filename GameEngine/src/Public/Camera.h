// Copyright Giorgio Gamba

#pragma once

class Camera
{

public:
	Camera()
	{
		lightPosition = glm::vec3(0.f, 0.f, 2.f);
		cameraPosition = glm::vec3(0.f, 0.f, 2.f);
	}

	void AddToShader(Shader* shader)
	{
		shader->AddUniformVector3fv(lightPosition, "lightPos");
		shader->AddUniformVector3fv(cameraPosition, "cameraPos");
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

	void CreateViewMatrix(Shader* shader)
	{
		// View matrix definition
		glm::vec3 CameraUpVector(0.f, 1.f, 0.f); // generic up vector
		glm::vec3 CameraFrontVector(0.f, 0.f, -1.f); // Bu CGI definition, camera looks at negative Z axis
		glm::vec3 CameraPosition(0.f, 0.f, 2.f);
		glm::mat4 ViewMatrix(1.f);
		ViewMatrix = glm::lookAt(CameraPosition, CameraPosition + CameraFrontVector, CameraUpVector);

		shader->AddUniformMatrix4fv(ViewMatrix, "ViewMatrix");
	}

	void UpdatePerspectiveMatrix(GLFWwindow* window, Shader* shader)
	{
		ProjectionMatrix = CreatePerspectiveMatrix(window);
		shader->AddUniformMatrix4fv(ProjectionMatrix, "ProjectionMatrix");
	}

private:
	
	glm::vec3 lightPosition;
	glm::vec3 cameraPosition;

	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;

};