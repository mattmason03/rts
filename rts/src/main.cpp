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
#include "TimeTypes.h"
#include "components\UnitComponents.h"

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
			x + (radius * cos(i *  twicePi / lineAmount + rot)),
			y + (radius* sin(i * twicePi / lineAmount + rot))
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

class MovementSystem {
public:
	ecs::EntityManager* manager;
	MovementSystem(ecs::EntityManager* manager) : manager{ manager } {};

	void Update() {
		manager->ForAll([this](ecs::Entity& entity, Transform& t, Destination& d, Radius& r) {
			auto orientation = t.dir * glm::dvec3(1., 0., 0.);
			auto angle = glm::orientedAngle(glm::normalize(glm::dvec2(orientation)), glm::normalize(glm::dvec2(d.pos - t.pos)));
			//auto angle = glm::angle(glm::normalize(orientation), glm::normalize(d.pos - t.pos));

			if (glm::abs(angle) > 0.001) {
				if (angle > .1)
					angle = .1;
				else if (angle < -.1)
					angle = -.1;
				t.dir *= glm::angleAxis(angle, glm::dvec3(0., 0., 1.));
			}
			else {
				if (glm::distance(t.pos, d.pos) < r.rad * .2) {
					t.pos = d.pos;
					entity.Remove<Destination>();
				}
				else {
					t.pos += r.rad * .2 * orientation;
				}
			}
		});
	}

};


class Selector {
public:
	struct Selected {
		bool selected = true;
	};

	ecs::EntityManager* manager;
	std::vector<ecs::Entity::Id> selectedEntities;

	Selector(ecs::EntityManager* manager) : manager{ manager } {};

	void Register() {
		Input::leftClicks.subscribe([this](MouseInfo& info) {
			HandleClick(info);
		});
		Input::rightClicks.subscribe([this](MouseInfo& info) {
			for (auto& id : selectedEntities) {
				auto pos = TranslatePos(glm::dvec2(info.position));
				manager->Add<Destination>(id, Destination{ glm::dvec3(pos, 0.) });
			}
		});
	}

	glm::dvec2 TranslatePos(glm::dvec2 screenPos) {
		//screenPos /= 640;
		//screenPos.x -= 1;
		//screenPos.y -= 1;
		//screenPos.y *= -1;
		return screenPos;
	}

	void HandleClick(MouseInfo& info) {
		FindEntity(TranslatePos(info.position));
	}

	void FindEntity(glm::dvec2 screenPos) {
		for (auto &id : selectedEntities) {
			manager->Remove<Selected>(id);
		}
		selectedEntities.clear();
		manager->ForAll([this, &screenPos](ecs::Entity& e, Transform& t, Radius& r) {
			auto delta = -screenPos + glm::dvec2(t.pos);
			auto distSquared = glm::dot(delta, delta);
			if (distSquared < r.rad * r.rad) {
				e.Add<Selected>();
				selectedEntities.push_back(e.GetId());
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
		manager.Create()
			.Add<Transform>(Transform{ glm::angleAxis(0., glm::dvec3(0, 0, 1)), glm::dvec3(0,0,0) })
			.Add<Sides>(4)
			.Add<Radius>(Radius{ 25. });
		manager.Create()
			.Add<Transform>(Transform{ glm::angleAxis(3.14, glm::dvec3(0, 0, 1)), glm::dvec3(320,320,0) })
			.Add<Sides>(5)
			.Add<Radius>(Radius{ 25. });

		Input::RegisterCallbacks(window);

		selector.Register();
	}

	void Update(Duration gameTime, Duration timeStep) override {
		movement.Update();
		//if (std::chrono::duration_cast<std::chrono::seconds>(gameTime).count() > 10)
		//	End();
	}

	void Render(Duration gameTime) override {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		manager.ForAll([this](ecs::Entity& entity, Transform& t, Sides& sides, Radius& r) {
			glColor3f(0, 1, 1);
			if (entity.Has<Selector::Selected>()) {
				glColor3f(1, 1, 0);
			}
			drawHollowCircle(t.pos.x, t.pos.y, r.rad, glm::angle(t.dir), sides.sides);
		});

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
};

int main(int argc, char** argv)
{
	spdlog::set_level(spdlog::level::err);
	std::cout << std::is_same<ecs::Entity::Id, ecs::Entity>() << std::endl;
	TestGame game;
	game.renderStep = Millis(20);
	game.updateStep = Millis(40);
	setExeWorkingDir(argv);

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);
	window = glfwCreateWindow(1000, 1000, "Simple example", NULL, NULL);
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
	gluOrtho2D(0, 1000, 1000, 0);

	game.Play();

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}