// General Utility class

#pragma once

#include <glm/glm.hpp>

#pragma region Definitions

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#pragma endregion

struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texcoord;
    glm::vec3 normal;
};