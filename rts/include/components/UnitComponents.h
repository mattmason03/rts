#pragma once

#include "glm/vec3.hpp"
#include "glm/gtc/quaternion.hpp"

struct Transform {
	glm::dquat dir;
	glm::dvec3 pos;
};

struct Sides {
	Sides(int sides) :sides(sides) {};
	int sides;
};

struct Destination {
	glm::dvec3 pos;
};