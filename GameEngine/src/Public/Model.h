// Copyright Giorgio Gamba

#pragma once

#pragma region Includes

#include <vector>

#include "Public/Mesh.h"

#pragma endregion

class Texture;
class Material;

class Model
{

#pragma region Lifecycle

public:

	Model(std::string InName, const glm::vec3& InPosition, Material* InMaterial, Texture* InOverrideTextureDiffuse, const std::vector<Mesh*> InMeshes)
	{
		this->Name = InName;

		Move(Position);

		this->Material = InMaterial;
		this->OverrideTextureDiffuse = InOverrideTextureDiffuse;
		this->Meshes = InMeshes;

		int MeshCounter = 0;
		for (Mesh* InMesh : InMeshes)
		{
			if (!InMesh)
				continue;

			// Model's meshes own the same name with a incrementing suffix
			InMesh->SetName(InName + "_" + std::to_string(MeshCounter));
			Meshes.push_back(InMesh);
		}
	}

	~Model();

private:

	std::string Name;

	std::vector<Mesh*> Meshes;

#pragma endregion

#pragma region Render

public:

	void Update(GLFWwindow* Window, Shader* Shader);

	void Render(Shader* Shader);

	void UpdateUniforms(Shader* Shader);

private:

	Material* Material;

	Texture* OverrideTextureDiffuse;

#pragma endregion

#pragma region Transforms

public:

	void Move(const glm::vec3& Movement);

	void Rotate(const glm::vec3& Rotation);

	void Scale(const glm::vec3& Scale);

private:

	glm::vec3 Position;

#pragma endregion

};
