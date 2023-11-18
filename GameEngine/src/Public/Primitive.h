#pragma once

#include <vector>

class Primitive
{
public:

	Primitive() {}
	~Primitive() {}

	void Initialize(Vertex* vertices, const unsigned NumVertices, GLuint* indices, const unsigned NumIndices)
	{
		for (size_t i = 0; i < NumVertices; i++)
		{
			this->vertices.push_back(vertices[i]);
		}

		for (size_t i = 0; i < NumIndices; i++)
		{
			this->indices.push_back(indices[i]);
		}
	}

	Vertex* GetVertices() { return this->vertices.data(); }
	GLuint* GetIndices() { return this->indices.data(); }

	unsigned GetNumVertices() const { return this->vertices.size(); }
	unsigned GetNumIndices() const { return this->indices.size(); }

private:

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

};

class Triangle : public Primitive
{
public:

	Triangle() : Primitive()
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

};

class Quad : public Primitive
{
public:

	Quad() : Primitive()
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
			0, 2, 3
		};

		unsigned numIndices = 6;

		Initialize(vertices, numVertices, indices, numIndices);
	}

};





