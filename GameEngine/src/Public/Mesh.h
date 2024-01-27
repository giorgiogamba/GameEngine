// Copyright Giorgio Gamba

#pragma region Includes

#pragma once

#include "Shader.h"
#include "Primitive.h"
#include "Vertex.h"

#pragma endregion

class Mesh
{

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

	void Update(GLFWwindow* window, Shader* shader)
	{
		if (!window || !shader)
			return;

		CreateModelMatrix();
		shader->AddUniformMatrix4fv(modelMatrix, "ModelMatrix");
	}

	void Draw(Shader* Shader)
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
				glDrawArrays(GL_TRIANGLES, 0, (unsigned) GetNumVertices());
			}
			else
			{
				glDrawElements(GL_TRIANGLES, (unsigned) GetNumIndices(), GL_UNSIGNED_INT, 0);
			}
		}
		glBindVertexArray(0);
		glUseProgram(0);
	}

	void Move(const glm::vec3& Movement)
	{
		this->position += Movement;
	}

	void Rotate(const glm::vec3& Rotation)
	{
		this->rotation += Rotation;
	}

	void Scale(const glm::vec3 Scale)
	{
		this->scale += Scale;
	}

	void SetName(const std::string& InName) { Name = InName; }
	std::string GetName() const { return Name; }

private:

	void InitializeMesh(Vertex* VertexArray, const unsigned& NumVertices, GLuint* IndicesArray, const unsigned& NumIndices)
	{
		InitVAO(VertexArray, NumVertices, IndicesArray, NumIndices);
		EnableVertexPointer();
		CreateModelMatrix();
	}

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

	//void UpdateInput(GLFWwindow* window)
	//{
	//	if (!window)
	//		return;
	//	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	//	{
	//		position.z -= 0.005f;
	//	}
	//	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	//	{
	//		position.z += 0.005f;
	//	}
	//	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	//	{
	//		position.x -= 0.005f;
	//	}
	//	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	//	{
	//		position.x += 0.005f;
	//	}
	//	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	//	{
	//		rotation.y -= 0.05f;
	//	}
	//	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	//	{
	//		rotation.y += 0.05f;
	//	}
	//}

	void InitVAO(Vertex* VertexArray, const unsigned& NumVertices, GLuint* IndicesArray, const unsigned& NumIndices)
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

	void CreateModelMatrix()
	{
		modelMatrix = glm::mat4(1.f); // Creates a diagonal identity matrix
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f)); // Rotation on the X axis
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f)); // Rotation on the Y axis
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f)); // Rotation on the Z axis
		modelMatrix = glm::scale(modelMatrix, scale);
	}

	Vertex* GetVertexArray() const { return VertexArray; }
	GLuint* GetIndicesArray() const { return IndicesArray; }
	const unsigned GetNumVertices() const { return NumVertices; }
	const unsigned GetNumIndices() const { return NumIndices; }

	glm::vec3 GetPosition() const { return position; }
	glm::vec3 GetRotation() const { return rotation; }
	glm::vec3 GetScale() const { return scale; }

	Primitive* Primitive;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 modelMatrix;

	Vertex* VertexArray;
	unsigned int NumVertices;
	GLuint* IndicesArray;
	unsigned int NumIndices;

	std::string Name;
};
