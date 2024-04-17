// Copyright (c) Giorgio Gamba

#pragma region Includes

#include <glm/glm.hpp>
#include <string>

#include "Public/Model.h"
#include "Public/Mesh.h"
#include "Public/Material.h"
#include "Public/Texture.h"

#pragma endregion

#pragma region Lifecycle

Model::~Model()
{
	for (Mesh*& Mesh : Meshes)
	{
		delete Mesh;
	}
}

#pragma endregion

#pragma region Render

void Model::Update(GLFWwindow* Window, Shader* Shader)
{
	for (Mesh* Mesh : Meshes)
	{
		if (!Mesh)
			continue;

		Mesh->Update(Window, Shader);
	}
}

void Model::Render(Shader * Shader)
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

void Model::UpdateUniforms(Shader* Shader)
{
	if (!Shader)
		return;

	Material->ApplyToShader(Shader);
	OverrideTextureDiffuse->ApplyTexture(Shader->GetID());
	OverrideTextureDiffuse->unuse();
}

#pragma endregion

#pragma region Transforms

void Model::Move(const glm::vec3 & Movement)
{
	for (Mesh* Mesh : Meshes)
	{
		if (!Mesh)
			continue;

		Mesh->Move(Movement);
	}
}

void Model::Rotate(const glm::vec3& Rotation)
{
	for (Mesh* Mesh : Meshes)
	{
		if (!Mesh)
			continue;

		Mesh->Rotate(Rotation);
	}
}

void Model::Scale(const glm::vec3& Scale)
{
	for (Mesh* Mesh : Meshes)
	{
		if (!Mesh)
			continue;

		Mesh->Scale(Scale);
	}
}

#pragma endregion
