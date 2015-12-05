#pragma once

#include "glm\vec3.hpp"
#include "glm\gtc\quaternion.hpp"
#include "Frame.h"

struct PositionTransform {
	PositionTransform(glm::quat orientation = glm::quat(), glm::vec3 position = glm::vec3(), Frame frame = Frame()) 
		: orientation(orientation), position(position), frame(frame) {}
	glm::quat orientation;
	glm::vec3 position;
	Frame frame;
};