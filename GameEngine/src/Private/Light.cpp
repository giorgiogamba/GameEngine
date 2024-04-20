// Copyright Giorgio Gamba

#pragma once

#include "Public/Light.h"

void PointLight::ApplyToShader(Shader* Shader)
{
	if (!Shader)
		return;

	Shader->AddUniformVector3fv(Position, "PointLight.position");
	Shader->AddUniformFloat(Intensity, "PointLight.intensity");
	Shader->AddUniformVector3fv(Color, "PointLight.color");
	Shader->AddUniformFloat(Constant, "PointLight.constant");
	Shader->AddUniformFloat(Linear, "PointLight.linear");
	Shader->AddUniformFloat(Quadratic, "PointLight.quadratic");

}
