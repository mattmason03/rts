#pragma once

#include "rxcpp\rx.hpp"
#include "glm\vec2.hpp"
#include "GLFW\glfw3.h"

namespace rx = rxcpp;

struct MouseInfo {
	int button, action, mods;
	glm::dvec2 position;
};

class Input {
public:
	static void RegisterCallbacks(GLFWwindow* window);
	static void MouseEvent(GLFWwindow* window, int button, int action, int mods);

	static rx::observable<MouseInfo> leftClicks;
	static rx::observable<MouseInfo> rightClicks;
	static rx::observable<MouseInfo> doubleClicks;

private:
	static rx::subjects::subject<MouseInfo> sub;
};