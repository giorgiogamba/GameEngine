// Copyright (c) 34BigThings, S.r.l. All rights reserved.

#include "Mesh.h"

#pragma region Lifecycle

void Mesh::Update(GLFWwindow* window, Shader* shader)
{
	if (!window || !shader)
		return;

	CreateModelMatrix();
	shader->AddUniformMatrix4fv(modelMatrix, "ModelMatrix");
}

void Mesh::Draw(Shader* Shader)
{
	Shader->use();
	glBindVertexArray(VAO);

	if (Primitive)
	{
		if (Primitive->GetNumIndices() == 0)
		{
			glDrawArrays(GL_TRIANGLES, 0, (unsigned)Primitive->GetNumVertices());
		}
		else
		{
			glDrawElements(GL_TRIANGLES, (unsigned)Primitive->GetNumIndices(), GL_UNSIGNED_INT, 0);
		}
	}
	else
	{
		if (NumIndices == 0)
		{
			glDrawArrays(GL_TRIANGLES, 0, (unsigned)GetNumVertices());
		}
		else
		{
			glDrawElements(GL_TRIANGLES, (unsigned)GetNumIndices(), GL_UNSIGNED_INT, 0);
		}
	}
	glBindVertexArray(0);
	glUseProgram(0);
}

#pragma endregion

#pragma region Rendering

void Mesh::InitializeMesh(Vertex* VertexArray, const unsigned& NumVertices, GLuint* IndicesArray, const unsigned& NumIndices)
{
	InitVAO(VertexArray, NumVertices, IndicesArray, NumIndices);
	EnableVertexPointer();
	CreateModelMatrix();
}

void Mesh::EnableVertexPointer()
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

void Mesh::InitVAO(Vertex* VertexArray, const unsigned& NumVertices, GLuint* IndicesArray, const unsigned& NumIndices)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * NumVertices, VertexArray, GL_STATIC_DRAW);

	if (NumVertices > 0)
	{
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * NumIndices, IndicesArray, GL_STATIC_DRAW);
	}
}

void Mesh::CreateModelMatrix()
{
	modelMatrix = glm::mat4(1.f); // Creates a diagonal identity matrix
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f)); // Rotation on the X axis
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f)); // Rotation on the Y axis
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f)); // Rotation on the Z axis
	modelMatrix = glm::scale(modelMatrix, scale);
}

#pragma endregion

