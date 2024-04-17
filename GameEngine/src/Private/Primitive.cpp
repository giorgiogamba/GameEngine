// Copyright (c) Giorgio Gamba

#include <GL/glew.h>

#include "Public/Primitive.h"
#include "Public/Vertex.h"

#pragma region Primitive

void Primitive::Initialize(Vertex* vertices, const unsigned NumVertices, GLuint* indices, const unsigned NumIndices)
{
	if (!vertices)
		return;

	for (size_t i = 0; i < NumVertices; i++)
	{
		this->vertices.push_back(vertices[i]);
	}

	for (size_t i = 0; i < NumIndices; i++)
	{
		this->indices.push_back(indices[i]);
	}
}

#pragma endregion

#pragma region Triangle

Triangle::Triangle() : Primitive()
{
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
	};

	unsigned numIndices = 6;

	Initialize(vertices, numVertices, indices, numIndices);
}

#pragma endregion

#pragma region Quad

Quad::Quad() : Primitive()
{
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
		0, 1, 2, // Triangle 1
		0, 2, 3  // Triangle 2
	};

	unsigned numIndices = 6;

	Initialize(vertices, numVertices, indices, numIndices);
}

#pragma endregion

#pragma region Pyramid

Pyramid::Pyramid() : Primitive()
{
	// Basic 2 triangles definition
	Vertex vertices[] =
	{
		glm::vec3(0.0f, 0.5f, 0.0f),		glm::vec3(1.0f, 0.0f, 0.0f),    glm::vec2(0.5f, 1.0f), glm::vec3(0.f, 0.f, 1.f),
		glm::vec3(-0.5f, -0.5f, 0.5f),      glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(0.0f, 0.0f), glm::vec3(0.f, 0.f, 1.f),
		glm::vec3(0.5f, -0.5f, 0.5f),       glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(1.0f, 0.0f), glm::vec3(0.f, 0.f, 1.f),

		glm::vec3(0.0f, 0.5f, 0.0f),		glm::vec3(1.0f, 1.0f, 0.0f),    glm::vec2(0.5f, 1.0f), glm::vec3(-1.f, 0.f, 0.f),
		glm::vec3(-0.5f, -0.5f, -0.5f),     glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(0.0f, 0.0f), glm::vec3(-1.f, 0.f, 0.f),
		glm::vec3(-0.5f, -0.5f, 0.5f),      glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(1.0f, 0.0f), glm::vec3(-1.f, 0.f, 0.f),

		glm::vec3(0.0f, 0.5f, 0.0f),		glm::vec3(1.0f, 1.0f, 0.0f),    glm::vec2(0.5f, 1.0f), glm::vec3(0.f, 0.f, -1.f),
		glm::vec3(0.5f, -0.5f, -0.5f),      glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(0.0f, 0.0f), glm::vec3(0.f, 0.f, -1.f),
		glm::vec3(-0.5f, -0.5f, -0.5f),     glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(1.0f, 0.0f), glm::vec3(0.f, 0.f, -1.f),

		glm::vec3(0.0f, 0.5f, 0.0f),		glm::vec3(1.0f, 1.0f, 0.0f),    glm::vec2(0.5f, 1.0f), glm::vec3(1.f, 0.f, 0.f),
		glm::vec3(0.5f, -0.5f, 0.5f),		glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(0.0f, 0.0f), glm::vec3(1.f, 0.f, 0.f),
		glm::vec3(0.5f, -0.5f, -0.5f),      glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(1.0f, 0.0f), glm::vec3(1.f, 0.f, 0.f),
	};

	unsigned numVertices = sizeof(vertices) / sizeof(Vertex);

	Initialize(vertices, numVertices, NULL, 0);
}

#pragma endregion
