// Copyright Giorgio Gamba

#pragma once

#include "Shader.h"

#pragma region Triangle Vertices

// Basic 2 triangles definition
Vertex vertices[] =
{
	glm::vec3(-0.5f, 0.5f, 0.0f),       glm::vec3(1.0f, 0.0f, 0.0f),    glm::vec2(0.0f, 1.0f), glm::vec3(0.f, 0.f, 1.f),
	glm::vec3(-0.5f, -0.5f, 0.0f),      glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(0.0f, 0.0f), glm::vec3(0.f, 0.f, 1.f),
	glm::vec3(0.5f, -0.5f, 0.0f),       glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(1.0f, 0.0f), glm::vec3(0.f, 0.f, 1.f),
	glm::vec3(0.5f, 0.5f, 0.0f),        glm::vec3(1.0f, 1.0f, 0.0f),    glm::vec2(1.0f, 1.0f), glm::vec3(0.f, 0.f, 1.f),
};

unsigned numVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3
};

unsigned numIndices = 6;

#pragma endregion

class Mesh
{

public:

	Mesh()
	{
		position = glm::vec3(0.f);
		rotation = glm::vec3(0.f);
		scale = glm::vec3(1.f);

		InitVAO();
		EnableVertexPointer();
		CreateModelMatrix();
	}

	~Mesh()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	void Update(GLFWwindow* window, Shader* shader)
	{
		UpdateInput(window);
		CreateModelMatrix();
		shader->AddUniformMatrix4fv(modelMatrix, "ModelMatrix");
	}

	void Draw()
	{
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
	}

private:

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

		glBindVertexArray(0);
	}

	void UpdateInput(GLFWwindow* window)
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

	void InitVAO()
	{
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}

	void CreateModelMatrix()
	{
		modelMatrix = glm::mat4(1.f); // Creates a diagonal identity matrix
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f)); // Rotation on the X axis
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f)); // Rotation on the Y axis
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f)); // Rotation on the Z axis
		modelMatrix = glm::scale(modelMatrix, scale);
	}

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 modelMatrix;
};
