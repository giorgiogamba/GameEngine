// Copyright Giorgio Gamba

#pragma once

#include <glm/glm.hpp>

#include "Public/Shader.h"

class Light
{
	
public:

	Light(const float InIntensity, const glm::vec3& InColor)
	{
		this->Intensity = InIntensity;
		this->Color = InColor;
	}

	~Light()
	{}

	virtual void ApplyToShader(Shader* shader) = 0;

protected:

	float Intensity;
	glm::vec3 Color;

};

class PointLight : public Light
{

public:

	PointLight( const glm::vec3 InPosition
			  , const float InIntensity = 1.f
			  , const glm::vec3& InColor = glm::vec3(1.f)
			  , float Constant = 1.f
			  , float Linear = 0.045f
			  , float Quadratic = 0.0075f)
		: Light(InIntensity, InColor)
	{
		this->Position = InPosition;
		this->Constant = Constant;
		this->Linear = Linear;
		this->Quadratic = Quadratic;
	}

	virtual void ApplyToShader(Shader* shader) override;

private:

	glm::vec3 Position;

	float Constant;

	float Linear;

	float Quadratic;


};
