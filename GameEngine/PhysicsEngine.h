// Copyright Giorgio Gamba

#pragma once

#include "src/Public/Mesh.h"

// Implementation of the physivs of a rigib body
// https://www.youtube.com/watch?v=4r_EvmPKOvY

struct RigidBody
{
	Mesh* Mesh;

	glm::vec3 X; // Center of mass
	glm::vec3 V; // Velocity 
	glm::mat3 R; // Rotation Matrix
	glm::vec3 Omega; // Angular momentum, the axis where to body rotates around

};

// Supposed we use a cuboid

