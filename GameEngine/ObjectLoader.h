// Copyright Giorgio Gamba

#pragma region Includes

#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <GL/GL.h>
#include <sstream>
#include <iostream>

#include "src/Public/Vertex.h"

#pragma endregion

/**
	This class wants to create a model from a Obj file, which is a file that lists a series of data
	on each line, using a prefix at the start that indicates which kind of data is shown.
*/

static std::vector<Vertex> LoadObject(const char* filename)
{
	std::vector<glm::vec3> Positions;
	std::vector<glm::vec2> TextureCoords;
	std::vector<glm::vec3> Normals;

	// Faces
	std::vector<GLuint> Positions_Indices;
	std::vector<GLuint> TextureCoords_Indices;
	std::vector<GLuint> Normals_Indices;

	std::vector<Vertex> Vertices;

	std::stringstream ss;
	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cout << "Error while loading object " << filename << ", error while opening the file" << std::endl;
		return Vertices;
	}

	std::string prefix = "";
	std::string line = "";

	while (std::getline(file, line))
	{
		ss.clear();
		ss.str(line);
		ss >> prefix;

		if (prefix == "v") // Vertex
		{
			glm::vec3 Position(0);
			ss >> Position.x >> Position.y >> Position.z;
			Positions.push_back(Position);
		}
		else if (prefix == "vt") // Texture coordinates
		{
			glm::vec2 TextureCoord(0);
			ss >> TextureCoord.x >> TextureCoord.y;
			TextureCoords.push_back(TextureCoord);
		}
		else if (prefix == "vn") // Normals
		{
			glm::vec3 Normal(0);
			ss >> Normal.x >> Normal.y >> Normal.z;
			Normals.push_back(Normal);
		}
	}

	// Summary
	std::cout << "Loading summary" << std::endl;
	std::cout << "Number of loaded vertices " << Positions.size() << std::endl;
	std::cout << "Number of loaded texture coordinates " << TextureCoords.size() << std::endl;
	std::cout << "Number of loaded normals " << Normals.size() << std::endl;

	return Vertices;
}
