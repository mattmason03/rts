#include "Input.h"

rx::subjects::subject<MouseInfo> Input::sub;
rx::subjects::subject<uint32_t> Input::keySub;
rx::observable<MouseInfo> Input::clickEvent;
rx::observable<MouseInfo> Input::rightClicks;
rx::observable<MouseInfo> Input::leftClicks;
rx::observable<MouseInfo> Input::doubleClicks;
rx::observable<char> Input::keys;
GLFWwindow* Input::window_;

void Input::MouseEvent(GLFWwindow* window, int button, int action, int mods) {
	sub.get_subscriber().on_next(MouseInfo{ button, action, mods, Input::GetCursorPos() });
}

void Input::TextEvent(GLFWwindow* window, unsigned int codepoint) {
	keySub.get_subscriber().on_next(codepoint);
}

glm::dvec2 Input::GetCursorPos() {
	glm::dvec2 pos;
	glfwGetCursorPos(window_, &pos.x, &pos.y);
	return pos;
}

void Input::RegisterCallbacks(GLFWwindow* window) {
	window_ = window;
	glfwSetMouseButtonCallback(window, &Input::MouseEvent);
	glfwSetCharCallback(window, &Input::TextEvent);

	clickEvent = sub.get_observable();
	auto clicks = clickEvent.filter([](const MouseInfo& m) { return m.action == GLFW_PRESS; });
	rightClicks = clicks.filter([](const MouseInfo& m) { return m.button == GLFW_MOUSE_BUTTON_RIGHT; });
	leftClicks = clicks.filter([](const MouseInfo& m) { return m.button == GLFW_MOUSE_BUTTON_LEFT; });
	doubleClicks = leftClicks.buffer_with_time_or_count(std::chrono::milliseconds(500), 2, rx::observe_on_event_loop())
		.filter([](const std::vector<MouseInfo> &v) { return v.size() == 2; })
		.map([](const std::vector<MouseInfo> &v) { return v[1]; });
	keys = keySub.get_observable().map([](const uint32_t utfKey) { return static_cast<char>(utfKey); });
}