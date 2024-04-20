// Copyright Giorgio Gamba

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/common.hpp>

#include "src/Public/Shader.h"

class Light
{

public:

	Light(const float InIntensity, const glm::vec3 InColor)
	{
		Intensity = InIntensity;
		Color = InColor;
	}

	~Light()
	{
	
	}

	virtual void SendToShader(Shader* Shader) = 0;

protected:

	float Intensity;
	glm::vec3 Color;

};

class PointLight : public Light
{
public:

	PointLight( const glm::vec3 InPosition
		, const float InIntensity = 1.f
		, const glm::vec3 InColor = glm::vec3(1.f)
		, const float InConstant = 1.f, const float InLinear = 0.045f
		, const float InQuadratic = 0.0075f )
		:Light(InIntensity, InColor)
	{
		Position = InPosition;
		Constant = InConstant;
		Linear = InLinear;
		Quadratic = InQuadratic;
	}

	~PointLight()
	{
	
	}

	void SetPosition(const glm::vec3 InPosition)
	{
		this->Position = InPosition;
	}

	void SendToShader(Shader* Shader) override
	{
		Shader->AddUniformVector3fv(this->Position, "PointLight.Position");
		Shader->AddUniformF(this->Intensity, "PointLight.Intensity");
		Shader->AddUniformVector3fv(this->Color, "PointLight.Color");
		Shader->AddUniformF(this->Constant, "PointLight.Constant");
		Shader->AddUniformF(this->Linear, "PointLight.Linear");
		Shader->AddUniformF(this->Quadratic, "PointLight.Quadratic");
	}

private:

	glm::vec3 Position;
	float Constant;
	float Linear;
	float Quadratic;

};
