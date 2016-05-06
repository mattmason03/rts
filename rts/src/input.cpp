#include "Input.h"

rx::subjects::subject<MouseInfo> Input::sub;
rx::observable<MouseInfo> Input::rightClicks;
rx::observable<MouseInfo> Input::leftClicks;
rx::observable<MouseInfo> Input::doubleClicks;

void Input::MouseEvent(GLFWwindow* window, int button, int action, int mods) {
	glm::dvec2 pos;
	glfwGetCursorPos(window, &pos.x, &pos.y);
	sub.get_subscriber().on_next(MouseInfo{ button, action, mods, pos });
}

void Input::RegisterCallbacks(GLFWwindow* window) {
	glfwSetMouseButtonCallback(window, &Input::MouseEvent);

	auto clicks = sub.get_observable().filter([](const MouseInfo& m) { return m.action == GLFW_PRESS; });
	rightClicks = clicks.filter([](const MouseInfo& m) { return m.button == GLFW_MOUSE_BUTTON_RIGHT; });
	leftClicks = clicks.filter([](const MouseInfo& m) { return m.button == GLFW_MOUSE_BUTTON_LEFT; });
	doubleClicks = leftClicks.buffer_with_time_or_count(std::chrono::milliseconds(500), 2, rx::observe_on_event_loop())
		.filter([](const std::vector<MouseInfo> &v) { return v.size() == 2; })
		.map([](const std::vector<MouseInfo> &v) { return v[1]; });
}