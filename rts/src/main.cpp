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

#include "rxcpp\rx.hpp"
#include "glm\vec3.hpp"
#include "glm\gtx\quaternion.hpp"
#include "glm\gtx\vector_angle.hpp"

#include "Game.h"
#include "Input.h"

namespace rx = rxcpp;
namespace rxu = rxcpp::util;

static void error_callback(int error, const char* description)
{
	LOG("GLFW", "Code " + error + std::string(description));
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

	GLfloat twicePi = 2.0f * glm::pi<float>();

	glBegin(GL_LINE_LOOP);
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
GLFWwindow* window;

struct Position {
	Position(double x, double y) : pos(x, y) {};
	Position(glm::dvec2 vec) : pos(vec) {};
	glm::dvec2 pos;
};

struct Transform {
	glm::dquat dir;
	glm::dvec3 pos;
};

struct Sides {
	Sides(int sides) :sides(sides) {};
	int sides;
};

struct Destination {
	glm::dvec3 pos;
};

class Selector {
public: 
	struct Selected {
		bool selected = true;
	};

	ecs::EntityManager* manager;
	std::vector<ecs::Entity::Id*> selectedEntities;

	Selector(ecs::EntityManager* manager) : manager{ manager } {};

	void Register() {
		Input::leftClicks.subscribe([this](MouseInfo& info) {
			HandleClick(info);
		});
		Input::rightClicks.subscribe([this](MouseInfo& info) {
			for (auto& id : selectedEntities) {
				auto pos = TranslatePos({ info.position.x, info.position.y });
				Destination dest{ glm::dvec3(pos.x, pos.y, 0.) };
				manager->Add<Destination>(*id, dest);
			}
		});
	}

	glm::dvec2 TranslatePos(glm::dvec2 screenPos) {
		glfwGetCursorPos(window, &screenPos.x, &screenPos.y);
		screenPos /= 640;
		screenPos.x -= 1;
		screenPos.y -= 1;
		screenPos.y *= -1;
		return screenPos;
	}

	void HandleClick(MouseInfo& info) {
		FindEntity(TranslatePos(info.position));
	}

	void FindEntity(glm::dvec2 screenPos) {
		for (auto &id : selectedEntities) {
			manager->Remove<Selected>(*id);
		}
		selectedEntities.clear();
		manager->ForAll([this,&screenPos](ecs::Entity::Id *id, Transform *t) {
			auto delta = -screenPos + glm::dvec2{ t->pos.x, t->pos.y };
			auto distSquared = glm::dot(delta, delta);
			if (distSquared < .1f) {
				manager->Add<Selected>(*id);
				selectedEntities.push_back(id);
			}
		});
	}
};

class MovementSystem {
public:
	ecs::EntityManager* manager;
	MovementSystem(ecs::EntityManager* manager) : manager{ manager } {};

	void Update() {
		manager->ForAll([this](ecs::Entity::Id* id, Transform* t, Destination* d) {
			auto orientation = t->dir * glm::dvec3(1., 0., 0.);
			auto angle = glm::angle(glm::normalize(orientation), glm::normalize(d->pos - t->pos));

			if (angle > 0.001) {
				if (angle > .1)
					angle = .1;
				t->dir *= glm::angleAxis(angle, glm::dvec3(0, 0, 1));
			}
			else {
				if (glm::distance(t->pos, d->pos) < .05) {
					t->pos = d->pos;
					manager->Remove<Destination>(*id);
				}
				else {
					t->pos += .05 * orientation;
				}
			}
		});
	}

};

class TestGame : public Game {
public:
	ecs::EntityManager manager;
	Selector selector;
	MovementSystem movement;

	TestGame() : selector(&manager), movement(&manager) {};

	void Load() override {
		auto unit = manager.Create();
		unit.Add<Transform>(Transform{ glm::angleAxis(0., glm::dvec3(0, 0, 1)), glm::dvec3(0,0,0) });
		unit.Add<Sides>(4);
		unit = manager.Create();
		unit.Add<Transform>(Transform{ glm::angleAxis(3.14, glm::dvec3(0, 0, 1)), glm::dvec3(.5,.5,0) });
		unit.Add<Sides>(5);

		Input::RegisterCallbacks(window);

		selector.Register();
	}

	void Update(Game::Duration gameTime, Game::Duration timeStep) override {
		movement.Update();
		//if (std::chrono::duration_cast<std::chrono::seconds>(gameTime).count() > 10)
		//	End();
	}

	void Render(Game::Duration gameTime) override {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		manager.ForAll([this](ecs::Entity::Id *id, Transform *t, Sides *sides) {
			glColor3f(0, 1, 1);
			if (manager.Has<Selector::Selected>(*id)) {
				glColor3f(1, 1, 0);
			}
			drawHollowCircle(t->pos.x, t->pos.y, .1f, glm::angle(t->dir), sides->sides);
		});

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
};

int main(int argc, char** argv)
{
	spdlog::set_level(spdlog::level::err);

	TestGame game;
	game.renderStep = Game::Millis(20);
	game.updateStep = Game::Millis(40);
	setExeWorkingDir(argv);

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);
	window = glfwCreateWindow(1280, 1280, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

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

	game.Play();

	//while (!glfwWindowShouldClose(window))
	//{
	//	//batch.Begin();

	//	//batch.SetTexture(smile);

	//	//batch.Draw(glm::vec2(320, 320), glm::vec3(0,0,0), glm::vec2(1,1), glm::vec2(320, 320));
	//	//batch.Draw(glm::vec2(320, 320), glm::vec3(0, 0, 0), glm::vec2(.5f, .5f), glm::vec2(320, 320));

	//	//batch.End();


	//	//drawHollowCircle(0, 0, .1f, .45f);
	//	//drawHollowCircle(-.2, -.2, .1f, -.45f, 5);
	//	//drawHollowCircle(.2, .2, .1f, .8f, 3);


	//}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}