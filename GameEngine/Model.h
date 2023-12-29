// Copyright Giorgio Gamba

#pragma once

#pragma region Includes

#include <vector>

#include "src/Public/Material.h"
#include "src/Public/Texture.h"
#include "src/Public/Mesh.h"

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
		for (Mesh* InMesh : InMeshes)
		{
			Meshes.push_back(InMesh);
		}

		for (Mesh* Mesh : Meshes)
		{
			Mesh->Move(InPosition);
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
		Rotate(glm::vec3(0.f, 0.1f, 0.f));

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

		for (Mesh* Mesh : Meshes)
		{
			if (!Mesh)
				continue;

			Mesh->Draw();
		}

		UpdateUniforms(Shader);
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
