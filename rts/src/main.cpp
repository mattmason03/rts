#include <chrono>
#include <thread>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <direct.h>
#include <functional>
#include <cstring>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "render/Texture2D.h"
#include "render/Batch2D.h"

#include "utilities/utilities.h"
#include "Components\PositionCurve.h"
#include "ecs\ecs.h"

#include "Game.h"

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

void setExeWorkingDir(char** argv){
	char delim = '\\';
	char* lastDelim = strrchr(argv[0], delim);
	*lastDelim = '\0';
	chdir(argv[0]);
	*lastDelim = delim;
}

void drawHollowCircle(GLfloat x, GLfloat y, GLfloat radius, GLfloat rot, int lineAmount = 6) {
	int i;

						  //GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * glm::pi<float>();

	glBegin(GL_LINE_LOOP);
	glColor3f(0.5f, 0.0f, 1.0f);
	for (i = 0; i <= lineAmount; i++) {
		glVertex2f(
			x + (radius * cos(i *  twicePi / lineAmount)),
			y + (radius* sin(i * twicePi / lineAmount))
			);
	}
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(x, y);
	float rad = radius * 1.2f;
	glVertex2f(x + rad * cos(rot), y + rad * sin(rot));
	glEnd();
}

class TestGame : public Game {
	void Update(Game::Duration gameTime, Game::Duration timeStep) override {
		if (std::chrono::duration_cast<std::chrono::seconds>(gameTime).count() > 3)
			End();
	}

	void Render(Game::Duration gameTime) override {

	}
};

int main(int argc, char** argv)
{
	TestGame game;
	game.renderStep = Game::Millis(50);
	game.updateStep = Game::Millis(100);
	setExeWorkingDir(argv);

	GLFWwindow* window;

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

	std::cout << glGetString(GL_VERSION) << std::endl;

	//Batch2D batch;

	//Texture2D smile(R"(..\resources\smile.png)");
	//Texture2D square(R"(..\resources\red.png)");

	//game.Play();

	while (!glfwWindowShouldClose(window))
	{
		//batch.Begin();

		//batch.SetTexture(smile);

		//batch.Draw(glm::vec2(320, 320), glm::vec3(0,0,0), glm::vec2(1,1), glm::vec2(320, 320));
		//batch.Draw(glm::vec2(320, 320), glm::vec3(0, 0, 0), glm::vec2(.5f, .5f), glm::vec2(320, 320));

		//batch.End();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		drawHollowCircle(0, 0, .1f, .45f);
		drawHollowCircle(-.2, -.2, .1f, -.45f, 5);
		drawHollowCircle(.2, .2, .1f, .8f, 3);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}