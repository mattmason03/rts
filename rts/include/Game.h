#pragma once
#include <memory>

#include "utilities\Timer.h"
#include "spdlog\spdlog.h"
#include "utilities\WrapMembers.h"

class Game {
public:
	void Play();
	virtual void Load();
	virtual void Input(double gametime);
	virtual void Update(double gametime, double timestep);
	virtual void Render(double gametime);
	virtual void Unload();
	void End();

	double secondsPerUpdate_ = 1. / 20.;
	double secondsPerRender_ = 1. / 60.;
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
	Time nextUpdate_;
	Time nextRender_;
	Seconds gameTime_;

	// used for logging
	friend class WrapMemberFunctions<Game>;
	void LogMethodStart_();
	void LogMethodEnd_();
	WrapMemberFunctions<Game> instrument_ = WrapMemberFunctions<Game>(this, &Game::LogMethodStart_, &Game::LogMethodEnd_);

	std::string methodName_;
	Time methodStart_;
	Time methodEnd_;
};