#pragma once
#include <memory>
#include <chrono>

#include "spdlog\spdlog.h"
#include "utilities\WrapMembers.h"
#include "TimeTypes.h"

class Game {
public:
	Game();
	Game(Duration updateStep, Duration renderStep);

	void Play();
	virtual void Load();
	virtual void Input(Duration gameTime);
	virtual void Update(Duration gameTime, Duration timeStep);
	virtual void Render(Duration gameTime);
	virtual void Unload();
	void End();

	double gameSpeedMultiplier = 1;
	Duration updateStep;
	Duration renderStep;

	std::shared_ptr<spdlog::logger> console;

private:
	void Load_();
	void Input_();
	void Update_();
	void Render_();
	void Unload_();
	Duration GameTime_();

	bool gameOver_ = false;

	Time nextUpdate_;
	Time nextRender_;
	Duration gameTime_;

	void Sleep_(Duration duration);

	// logging
	template <typename ...Params, typename ...Args>
	void Instrument_(std::string functionName, void (Game::*func)(Params...), Args && ...args) {
		Time start = Clock::now();
		console->info("{:8} Begin", functionName);
		(this->*func)(std::forward<Args>(args)...);
		console->info("{:8} Elapsed: {:4.3}ms", functionName, std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(Clock::now() - start).count());
	};
};