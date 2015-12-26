#pragma once
#include "GLFW\glfw3.h"

class Mouse {
public:
	Mouse(GLFWwindow* window) : window_{ window } {};

	void Drag();

	void DoubleClick();

	void LeftClick();

	void RightClick();

private:
	GLFWwindow* window_;
};