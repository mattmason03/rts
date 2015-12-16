#pragma once
#include "utilities\Timer.h"
#include <memory>
#include "spdlog\spdlog.h"

class Game {
public:
	void Play();
	virtual void Load();
	virtual void Input(double gametime);
	virtual void Update(double gametime, double timestep);
	virtual void Render(double gametime);
	virtual void Unload();
	void End();

	double secondsPerUpdate_ = 1. / 2.;
	double secondsPerRender_ = 1. / 6.;
	double gameSpeedMultiplier_ = 1;
	std::shared_ptr<spdlog::logger> console;

private:
	void Load_();
	void Input_();
	void Update_();
	void Render_();
	void Unload_();
	double GameTime_();

	bool gameOver_ = false;
	Timer timer_;
	double lastUpdate_ = 0;
	double lastRender_ = 0;
	double gameTime_ = 0;
};