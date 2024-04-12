// Copyright Giorgio Gamba

#pragma once

#pragma region Includes

#include <vector>

#include "Public/Material.h"
#include "Public/Texture.h"
#include "Public/Mesh.h"

#pragma endregion

class Model
{

public:

	Model ( std::string InName
		, const glm::vec3& InPosition
		, Material* InMaterial
		, Texture* InOverrideTextureDiffuse
		, const std::vector<Mesh*> InMeshes )
		: Name(InName)
		, Position(InPosition)
		, Material(InMaterial)
		, OverrideTextureDiffuse(InOverrideTextureDiffuse)
	{
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

	~Model()
	{
		for (Mesh*& Mesh : Meshes)
		{
			delete Mesh;
		}
	}

	void Update(GLFWwindow* Window, Shader* Shader)
	{
		for (Mesh* Mesh : Meshes)
		{
			if (!Mesh)
				continue;

			Mesh->Update(Window, Shader);
		}
	}

	void Render(Shader* Shader)
	{
		if (!Shader)
			return;

		//UpdateUniforms(Shader); // White screen problems

		for (Mesh* Mesh : Meshes)
		{
			if (!Mesh)
				continue;

			Mesh->Draw(Shader);
		}
	}

	void UpdateUniforms(Shader* Shader)
	{
		if (!Shader)
			return;

		Material->ApplyToShader(Shader);
		OverrideTextureDiffuse->ApplyTexture(Shader->GetID());
		OverrideTextureDiffuse->unuse();
	}

	void Move(const glm::vec3& Movement)
	{
		for (Mesh* Mesh : Meshes)
		{
			if (!Mesh)
				continue;

			Mesh->Move(Movement);
		}
	}

	void Rotate(const glm::vec3& Rotation)
	{
		for (Mesh* Mesh : Meshes)
		{
			if (!Mesh)
				continue;

			Mesh->Rotate(Rotation);
		}
	}

	void Scale(const glm::vec3& Scale)
	{
		for (Mesh* Mesh : Meshes)
		{
			if (!Mesh)
				continue;

			Mesh->Scale(Scale);
		}
	}

private:

	std::string Name;

	Material* Material;
	Texture* OverrideTextureDiffuse;
	std::vector<Mesh*> Meshes;
	glm::vec3 Position;
};
