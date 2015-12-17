#include <algorithm>

#include "Game.h"
#include "GLFW\glfw3.h"

void Game::Play() {
	Load_();
	while (!gameOver_) {

		if (timer_.Past(nextUpdate_)) {
			Update_();
		}

		if (timer_.Past(nextRender_)) {
			Input_();
			Render_();
		}

		auto sleepTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::min(nextRender_, nextUpdate_) - timer_.Now());
		console->info("Sleeping for {}s", Seconds(sleepTime).count());
		std::this_thread::sleep_for(sleepTime);
	}
	Unload_();
}

void Game::Load_() {
	gameOver_ = false;
	gameTime_ = Seconds(0);
	timer_.Start();
	nextUpdate_ = nextRender_ = timer_.Now();

	console = spdlog::stdout_logger_mt("game");
	glfwInit();

	methodName_ = __FUNCTION__;
	instrument_->Load();
}

void Game::Load() {}

void Game::Input_() {
	methodName_ = __FUNCTION__;
	instrument_->Input(GameTime_());
}

void Game::Input(double gametime) {}

void Game::Update_() {
	nextUpdate_ = timer_.Now() + Seconds(secondsPerUpdate_);
	methodName_ = __FUNCTION__;
	instrument_->Update(gameTime_.count(), secondsPerUpdate_ * gameSpeedMultiplier_);
	gameTime_ = gameTime_ + Seconds(secondsPerUpdate_) * gameSpeedMultiplier_;
}

void Game::Update(double gametime, double timestep) {}

void Game::Render_() {
	nextRender_ = timer_.Now() + Seconds(secondsPerRender_);
	methodName_ = __FUNCTION__;
	instrument_->Render(GameTime_());
}

void Game::Render(double gametime) {}

void Game::Unload_() {
	methodName_ = __FUNCTION__;
	instrument_->Unload();
	glfwTerminate();
}
void Game::Unload() {}

void Game::End() {
	console->info("{:16}", __FUNCTION__);
	gameOver_ = true;
}

double Game::GameTime_() {
	return (gameTime_ + (timer_.Now() - nextUpdate_) * gameSpeedMultiplier_).count();
}

void Game::LogMethodStart_() {
	methodStart_ = timer_.Now();
	console->info("{:16} Begin", methodName_);
}

void Game::LogMethodEnd_() {
	methodEnd_ = timer_.Now();
	console->info("{:16} Elapsed: {:5.4f}", methodName_, Seconds(methodEnd_ - methodStart_).count());
}