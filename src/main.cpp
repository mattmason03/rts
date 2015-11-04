#include <chrono>
#include <thread>
#include <iostream>
#include <vector>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>

#include "render/Texture2D.h"
#include "render/Batch2D.h"

#include "utilities/utilities.h"

//template <typename T>
//struct Curve : ecs::Component<Curve<T>> {
//	Curve(T prev, T next) : prev(prev), next(next) {}
//	T prev, next;
//	template <typename U>
//	T mix(const U& interpolant) {
//		return glm::mix(prev, next, interpolant);
//	}
//};

static void error_callback(int error, const char* description)
{
	//LOG("GLFW", "Code " + error + std::string(description));
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
	std::cout << UtilInt();
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, key_callback);
	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef((float)glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
		glBegin(GL_TRIANGLES);
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(-0.6f, -0.4f, 0.f);
		glColor3f(0.f, 1.f, 0.f);
		glVertex3f(0.6f, -0.4f, 0.f);
		glColor3f(0.f, 0.f, 1.f);
		glVertex3f(0.f, 0.6f, 0.f);
		glEnd();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
	//GLenum status = glewInit();

	//if (status != GLEW_OK) {
	//	LOG("Glew", std::string((char*)glewGetErrorString(status)));
	//	fprintf(stderr, "%s\n", glewGetErrorString(status));
	//	exit(EXIT_FAILURE);
	//}

	//glfwSetKeyCallback(window, key_callback);
	//glfwSetMouseButtonCallback(window, mouse_callback);

	//Batch2D batch;
	//glfwSwapInterval(0);
	//double oldTime = 0;
	//float rot = 0;

	//struct CTex2D : ecs::Component<CTex2D> {
	//	CTex2D(Texture2D* texture) :texture(texture) {}
	//	Texture2D* texture;
	//};

	//Texture2D smile(R"(..\resources\smile.png)");
	//Texture2D square(R"(..\resources\red.png)");

	//ecs::EntityManager manager;

	//using posCurve = Curve<glm::vec2>;
	//using scaleCurve = Curve<glm::vec2>;
	//using rotCurve = Curve<glm::vec3>;

	//manager.RegisterComponent<posCurve>();
	//manager.RegisterComponent<scaleCurve>();
	//manager.RegisterComponent<rotCurve>();
	//manager.RegisterComponent<CTex2D>();

	//for (int i = 0; i < 60; i++) {
	//	ecs::Entity entity = manager.CreateEntity();
	//	entity.AddComponent<posCurve>(getV2(), getV2());
	//	entity.AddComponent<scaleCurve>(getV2() / 1000.f, getV2() / 1000.f);
	//	entity.AddComponent<rotCurve>(getV3() / 1000.f, getV3() / 1000.f);
	//	
	//	int x = rand() % 3;

	//	if (x)
	//		entity.AddComponent<CTex2D>(&smile);
	//	else
	//		entity.AddComponent<CTex2D>(&square);
	//}
	////ecs::Entity entity = manager.CreateEntity();
	////entity.AddComponent<Curve<glm::vec2>>(glm::vec2(0, 0), glm::vec2(800, 800));
	////entity.AddComponent<CTex2D>(&smile);

	////entity = manager.CreateEntity();
	////entity.AddComponent<Curve<glm::vec2>>(glm::vec2(400, 400), glm::vec2(800, 400));
	////entity.AddComponent<CTex2D>(&square);

	////entity = manager.CreateEntity();
	////entity.AddComponent<Curve<glm::vec2>>(glm::vec2(0, 800), glm::vec2(200, 800));
	////entity.AddComponent<CTex2D>(&smile);

	////entity = manager.CreateEntity();
	////entity.AddComponent<Curve<glm::vec2>>(glm::vec2(600, 600), glm::vec2(200, 100));
	////entity.AddComponent<CTex2D>(&smile);

	//float frameCount = 0;
	//while (!glfwWindowShouldClose(window))
	//{
	//	LOG("main", "loop begin");

	//	double currentTime = glfwGetTime();
	//	oldTime = currentTime;
	//	const GLfloat color[] = { (float)sin(currentTime) * 0.5f + 0.5f,
	//		(float)cos(currentTime) * 0.5f + 0.5f,
	//		0.0f, 1.0f };

	//	const GLfloat col[4] = { (float)sin(currentTime) * 0.5f + 0.5f, (float)sin(currentTime) * 0.5f + 0.5f, (float)sin(currentTime) * .5f + 0.5f, 1.f };

	//	glClearBufferfv(GL_COLOR, 0, col);

	//	batch.Begin();

	//	for (auto& e : manager.GetEntities<CTex2D, Curve<glm::vec2>>())
	//	{
	//		int x = posCurve::componentId;
	//		int y = scaleCurve::componentId;
	//		batch.SetTexture(*e.GetComponent<CTex2D>()->texture);
	//		batch.Draw(e.GetComponent<posCurve>()->mix((float)sin(currentTime) * .5f + .5f),
	//			e.GetComponent<rotCurve>()->mix((float)sin(currentTime) * .5f + .5f),
	//			glm::vec2(0.2f, 0.2f));
	//	}

	//	batch.End();
	//	glfwSwapBuffers(window);
	//	glfwPollEvents();
	//	rot = glm::mod(rot + 10.f, 360.f);


	//	std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 30));
	//	frameCount++;
	//}
}
//static void error_callback(int error, const char* description)
//{
//	fputs(description, stderr);
//}
//static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
//{
//	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, GL_TRUE);
//}
//int main(void)
//{
//	GLFWwindow* window;
//	glfwSetErrorCallback(error_callback);
//	if (!glfwInit())
//		exit(EXIT_FAILURE);
//	window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
//	if (!window)
//	{
//		glfwTerminate();
//		exit(EXIT_FAILURE);
//	}
//	glfwMakeContextCurrent(window);
//	glfwSwapInterval(1);
//	glfwSetKeyCallback(window, key_callback);
//	while (!glfwWindowShouldClose(window))
//	{
//		float ratio;
//		int width, height;
//		glfwGetFramebufferSize(window, &width, &height);
//		ratio = width / (float)height;
//		glViewport(0, 0, width, height);
//		glClear(GL_COLOR_BUFFER_BIT);
//		glMatrixMode(GL_PROJECTION);
//		glLoadIdentity();
//		glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
//		glMatrixMode(GL_MODELVIEW);
//		glLoadIdentity();
//		glRotatef((float)glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
//		glBegin(GL_TRIANGLES);
//		glColor3f(1.f, 0.f, 0.f);
//		glVertex3f(-0.6f, -0.4f, 0.f);
//		glColor3f(0.f, 1.f, 0.f);
//		glVertex3f(0.6f, -0.4f, 0.f);
//		glColor3f(0.f, 0.f, 1.f);
//		glVertex3f(0.f, 0.6f, 0.f);
//		glEnd();
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//	}
//	glfwDestroyWindow(window);
//	glfwTerminate();
//	exit(EXIT_SUCCESS);
//}
