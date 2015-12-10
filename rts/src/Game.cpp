#include "Game.h"

void Game::Play() {
	Load();

	gameOver_ = false;
	while (!gameOver_) {
		
	}
}

void Game::Load() {}

void Game::Update() {}

void Game::Render() {}

void Game::Unload() {}

void Game::End() {
	gameOver_ = true;
}