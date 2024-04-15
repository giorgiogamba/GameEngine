// Copyright Giorgio Gamba

#pragma region Includes

#pragma once

#include "Shader.h"
#include "Primitive.h"
#include "Vertex.h"

#pragma endregion

class Mesh
{

#pragma region Lifecycle

public:

	Mesh(Primitive* Primitive)
	{
		if (!Primitive)
			return;

		this->Primitive = Primitive;

		this->VAO = 0;
		this->VBO = 0;
		this->EBO = 0;

		this->position = glm::vec3(0.f);
		this->rotation = glm::vec3(0.f);
		this->scale = glm::vec3(1.f);

		InitializeMesh(Primitive->GetVertices(), Primitive->GetNumVertices(), Primitive->GetIndices(), Primitive->GetNumIndices());
	}

	Mesh( Vertex* VertexArray
		, const unsigned& NumVertices
		, GLuint* IndicesArray
		, const unsigned& NumIndices
		, const glm::vec3& Position
		, const glm::vec3 Rotation
		, const glm::vec3& Scale )
	{
		this->NumVertices = NumVertices;
		this->NumIndices = NumIndices;

		this->VertexArray = new Vertex[this->NumVertices];
		for (size_t i = 0; i < this->NumVertices; i++)
		{
			this->VertexArray[i] = VertexArray[i];
		}

		this->IndicesArray = new GLuint[this->NumIndices];
		for (size_t i = 0; i < this->NumIndices; i++)
		{
			this->IndicesArray[i] = IndicesArray[i];
		}

		this->position = Position;
		this->rotation = Rotation;
		this->scale = Scale;

		InitializeMesh(VertexArray, NumVertices, IndicesArray, NumIndices);
	}

	Mesh(const Mesh& obj)
	{
		this->NumVertices = obj.GetNumVertices();
		this->NumIndices = obj.GetNumIndices();

		this->VertexArray = new Vertex[this->NumVertices];
		for (size_t i = 0; i < this->NumVertices; i++)
		{
			this->VertexArray[i] = obj.GetVertexArray()[i];
		}

		this->IndicesArray = new GLuint[this->NumIndices];
		for (size_t i = 0; i < this->NumIndices; i++)
		{
			this->IndicesArray[i] = obj.GetIndicesArray()[i];
		}

		this->position = obj.GetPosition();
		this->rotation = obj.GetRotation();
		this->scale = obj.GetScale();

		InitializeMesh(VertexArray, NumVertices, IndicesArray, NumIndices);
	}
		
	~Mesh()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);

		if (GetNumVertices() > 0)
		{
			glDeleteBuffers(1, &EBO);
		}

		delete[] this->VertexArray;
		delete[] this->IndicesArray;
	}

	void Update(GLFWwindow* window, Shader* shader);

	void Draw(Shader* Shader);

#pragma endregion

#pragma region Transforms

public:

	void Move(const glm::vec3& Movement) { this->position += Movement; }
	void Rotate(const glm::vec3& Rotation) { this->rotation += Rotation; }
	void Scale(const glm::vec3 Scale) { this->scale += Scale; }

	glm::vec3 GetPosition() const { return position; }
	glm::vec3 GetRotation() const { return rotation; }
	glm::vec3 GetScale() const { return scale; }

private:

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

#pragma endregion

#pragma region General Information

public:

	void SetName(const std::string& InName) { Name = InName; }
	std::string GetName() const { return Name; }

private:

	std::string Name;

#pragma endregion

#pragma region Rendering

private:

	void InitializeMesh(Vertex* VertexArray, const unsigned& NumVertices, GLuint* IndicesArray, const unsigned& NumIndices);

	void EnableVertexPointer();

	void InitVAO(Vertex* VertexArray, const unsigned& NumVertices, GLuint* IndicesArray, const unsigned& NumIndices);

	void CreateModelMatrix();

	Vertex* GetVertexArray() const { return VertexArray; }
	GLuint* GetIndicesArray() const { return IndicesArray; }
	const unsigned GetNumVertices() const { return NumVertices; }
	const unsigned GetNumIndices() const { return NumIndices; }

	Primitive* Primitive;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	glm::mat4 modelMatrix;

	Vertex* VertexArray;
	unsigned int NumVertices;
	GLuint* IndicesArray;
	unsigned int NumIndices;

#pragma endregion

};
