#pragma once
#include "glm\vec3.hpp"
#include "glm\vec2.hpp"

template <typename T>
class Grid {
public:
	Grid(glm::ivec2 partitions, glm::dvec2 lowerBound, glm::dvec2 upperBound);

	Insert(T item, glm::dvec3 pos);
};