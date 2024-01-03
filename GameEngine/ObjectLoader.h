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
	// Atomic elements
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
		else if (prefix == "f")
		{
			int counter = 0;
			GLuint TempGLuint = 0;
			while (ss >> TempGLuint)
			{
				if (counter == 0) // Vertex
				{
					Positions_Indices.push_back(TempGLuint);
				}
				else if (counter == 1)
				{
					TextureCoords_Indices.push_back(TempGLuint);
				}
				else if (counter == 2)
				{
					Normals_Indices.push_back(TempGLuint);
				}

				if (ss.peek() == '/')
				{
					ss.ignore(1, '/');
					counter++;
				}
				else if (ss.peek() == ' ')
				{
					ss.ignore(1, ' ');
					counter++;
				}

				if (counter > 2)
					counter = 0;
			}
		}
		else
		{

		}
	}

	// Build vertices array
	Vertices.resize(Positions_Indices.size(), Vertex()); // Fill with empty Vertices
	for (size_t i = 0; i < Vertices.size(); ++i)
	{
		Vertices[i].position = Positions[Positions_Indices[i] - 1];
		Vertices[i].texcoord = TextureCoords[TextureCoords_Indices[i] - 1];
		Vertices[i].normal = Normals[Normals_Indices[i] - 1];
		Vertices[i].color = glm::vec3(1.f, 1.f, 1.f);
	}

	// Summary
	std::cout << "Loading summary" << std::endl;
	std::cout << "Number of loaded vertices " << Positions.size() << std::endl;
	std::cout << "Number of loaded texture coordinates " << TextureCoords.size() << std::endl;
	std::cout << "Number of loaded normals " << Normals.size() << std::endl;

	return Vertices;
}
