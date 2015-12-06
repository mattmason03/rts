#include <chrono>
#include <thread>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <direct.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "render/Texture2D.h"
#include "render/Batch2D.h"

#include "utilities/utilities.h"
#include "Components\PositionCurve.h"
#include "ecs\ecs.h"

static void error_callback(int error, const char* description)
{
	LOG("GLFW", "Code " + error + std::string(description));
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

static void mouse_callback(GLFWwindow* window, int button, int action, int modifiers) {
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	if (action == GLFW_RELEASE) {
		std::cout << "mouse: release " << button << " at " << x << ":" << y << std::endl;
	}
	else {
		std::cout << "mouse: press " << button << " at " << x << ":" << y << std::endl;
	}
}

glm::vec3 getV3() {
	return glm::vec3(rand() % 1000, rand() % 1000, rand() % 1000);
}

glm::fvec2 getV2() {
	return static_cast<glm::vec2>(getV3());
}

void testStatic(int input) {
	static int print = input;
	std::cout << print;
}

int main(int argc, char** argv)
{
	GLFWwindow* window;

	std::cout << argv[0];

	std::string s(argv[0]);

	s[s.find_last_of('\\')] = '\0';

	std::cout << s.c_str();

	
	chdir(s.c_str());

	glfwSetErrorCallback(error_callback);



	if (!glfwInit())
		exit(EXIT_FAILURE);
	window = glfwCreateWindow(640, 640, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, key_callback);


	GLenum status = glewInit();

	if (status != GLEW_OK) {
		LOG("Glew", std::string((char*)glewGetErrorString(status)));
		fprintf(stderr, "%s\n", glewGetErrorString(status));
		char x;
		std::cin >> x;
		exit(EXIT_FAILURE);
	}

	Batch2D batch;

	Texture2D smile(R"(..\resources\smile.png)");
	Texture2D square(R"(..\resources\red.png)");

	while (!glfwWindowShouldClose(window))
	{
		batch.Begin();

		batch.SetTexture(smile);

		batch.Draw(glm::vec2(320, 320), glm::vec3(0,0,0), glm::vec2(1,1), glm::vec2(320, 320));
		batch.Draw(glm::vec2(320, 320), glm::vec3(0, 0, 0), glm::vec2(.5f, .5f), glm::vec2(320, 320));

		batch.End();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}