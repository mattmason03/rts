#include "Game.h"
#include "GLFW\glfw3.h"
#include "utilities\utilities.h"

void Game::Play() {
	Load_();
	while (!gameOver_) {

		if (timer_.Past(lastUpdate_ + secondsPerUpdate_)) {
			Input_();

			Update_();
		}

		if (timer_.Past(lastRender_ + secondsPerRender_)) {
			Render_();
		}
	}
	Unload_();
}

void Game::Load_() {
	gameOver_ = false;
	gameTime_ = 0;
	lastUpdate_ = 0;
	lastRender_ = 0;
	console = spdlog::stdout_logger_mt("console");
	glfwInit();
	Timer::SetTimeFunction(&glfwGetTime);
	timer_.Start();
	console->info("Load {:2}", timer_.Seconds());
	Load();
}

void Game::Load() {}

void Game::Input_() {
	double time = GameTime_();
	console->info("Input {:2}", time);
	Input(time);
}

void Game::Input(double gametime) {}

void Game::Update_() {
	lastUpdate_ = timer_.Seconds();
	console->info("Update {:2}", gameTime_);
	Update(gameTime_, secondsPerUpdate_ * gameSpeedMultiplier_);
	gameTime_ = gameTime_ + secondsPerUpdate_ * gameSpeedMultiplier_;
}

void Game::Update(double gametime, double timestep) {}

void Game::Render_() {
	lastRender_ = timer_.Seconds();
	double time = GameTime_();
	console->info("Render {:2}", time);
	Render(time);
}

void Game::Render(double gametime) {}

void Game::Unload_() {
	double time = GameTime_();
	console->info("Unload {:2}", time);
	Unload();
	glfwTerminate();
}
void Game::Unload() {}

void Game::End() {
	console->info("End {:2}", GameTime_());
	gameOver_ = true;
}

double Game::GameTime_() {
	return gameTime_ + (timer_.Seconds() - lastUpdate_) * gameSpeedMultiplier_;
}