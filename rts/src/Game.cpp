#include <algorithm>

#include "Game.h"
#include "GLFW\glfw3.h"

Game::Game()
	:Game(Millis(1000 / 20), Millis(1000 / 60)) {}

Game::Game(Duration updateStep, Duration renderStep)
	: updateStep{ updateStep }, renderStep{ renderStep } {}

void Game::Play() {
	Load_();
	while (!gameOver_) {
		if (Clock::now() > nextUpdate_) {
			Update_();
		}

		if (Clock::now() > nextRender_) {
			Input_();
			Render_();
		}

		Instrument_("Sleep", &Game::Sleep_, (std::min(nextRender_, nextUpdate_) - Clock::now()));
	}
	Unload_();
}

void Game::Load_() {
	gameOver_ = false;
	gameTime_ = Duration(0);
	nextUpdate_ = nextRender_ = Clock::now();

	console = spdlog::stdout_logger_mt("game");
	glfwInit();

	Instrument_("Load", &Game::Load);
}

void Game::Load() {}

void Game::Input_() {
	Instrument_("Input", &Game::Input, GameTime_());
}

void Game::Input(Duration gameTime) {}

void Game::Update_() {
	nextUpdate_ = Clock::now() + updateStep;
	auto timeStep = std::chrono::duration_cast<Duration>(gameSpeedMultiplier * updateStep);
	Instrument_("Update", &Game::Update, gameTime_, timeStep);
	gameTime_ = gameTime_ + timeStep;
}

void Game::Update(Duration gameTime, Duration timeStep) {}

void Game::Render_() {
	nextRender_ = Clock::now() + renderStep;
	Instrument_("Render", &Game::Render, GameTime_());
}

void Game::Render(Duration gameTime) {}

void Game::Unload_() {
	Instrument_("Unload", &Game::Unload);
	glfwTerminate();
}
void Game::Unload() {}

void Game::End() {
	console->info("{:8}", "End");
	gameOver_ = true;
}

Game::Duration Game::GameTime_() {
	return gameTime_ + std::chrono::duration_cast<Duration>((Clock::now() - nextUpdate_) * gameSpeedMultiplier);
}

void Game::Sleep_(Duration duration) {
	std::this_thread::sleep_for(duration);
}