#pragma once
#include "glm\glm.hpp"

#include <vector>

template <typename Stored>
class Grid {
public:
	Grid(glm::ivec2 partitions, glm::dvec2 lowerBound, glm::dvec2 upperBound);

	void Insert(Stored item, glm::dvec3 pos);

	void Clear();

	std::vector<std::vector<Stored>*> Within(glm::dvec3 pos, double radius);

private:
	std::vector<std::vector<Stored>> grid;
	glm::ivec2 partitions;
	glm::dvec2 squareDimensions;
};

template <typename Stored>
Grid<Stored>::Grid(glm::ivec2 partitions, glm::dvec2 lowerBound, glm::dvec2 upperBound): partitions(partitions) {
	glm::dvec2 boundDiff = upperBound - lowerBound;
	squareDimensions = boundDiff / glm::dvec2(partitions);
	int partitionCount = partitions.x * partitions.y;
	for (int i = 0; i < partitionCount; i++)
		grid.push_back(std::vector<Stored>());
}

template <typename Stored>
void Grid<Stored>::Insert(Stored item, glm::dvec3 pos) {
	glm::ivec2 indices = glm::ivec2(glm::dvec2(pos) / squareDimensions);
	grid[partitions.x * indices.y + indices.x].push_back(item);
}

template <typename Stored>
void Grid<Stored>::Clear() {
	for (auto& square : grid)
		square.clear();
}

template <typename Stored>
std::vector<std::vector<Stored>*> Grid<Stored>::Within(glm::dvec3 pos, double radius) {
	std::vector<std::vector<Stored>*> squares;

	auto posUpLeft = glm::dvec2(pos) - .5 * squareDimensions;
	
	auto gridRad = glm::distance(.5 * squareDimensions, glm::dvec2{ 0,0 });
	auto computedRad = gridRad + radius;
	auto radSquared = computedRad * computedRad;

	for (int i = 0; i < partitions.x; i++) {
		for (int j = 0; j < partitions.y; j++) {
			auto gridSquareUpLeft = glm::dvec2(i, j) * squareDimensions;
			auto diff = posUpLeft - gridSquareUpLeft;
			if (glm::dot(diff, diff) < radSquared)
				squares.push_back(&grid[partitions.x * j + i]);
		}
	}
	return squares;
}