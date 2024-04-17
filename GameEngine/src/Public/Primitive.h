// Copyright Giorgio Gamba

#pragma region Includes

#pragma once

#include <vector>
#include "GL/glew.h"

#include "Vertex.h"

#pragma endregion

class Primitive
{
public:

	Primitive() {}
	~Primitive() {}

	void Initialize(Vertex* vertices, const unsigned NumVertices, GLuint* indices, const unsigned NumIndices);

	Vertex* GetVertices() { return this->vertices.data(); }
	GLuint* GetIndices() { return this->indices.data(); }

	size_t GetNumVertices() const { return this->vertices.size(); }
	size_t GetNumIndices() const { return this->indices.size(); }

protected:

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

};

class Triangle : public Primitive
{
public:

	Triangle();

};

class Quad : public Primitive
{
public:

	Quad();

};

class Pyramid : public Primitive
{
public:

	Pyramid();

};





