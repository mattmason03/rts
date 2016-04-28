#pragma once

#include <chrono>

using Seconds = std::chrono::duration<double>;
using Clock = std::chrono::high_resolution_clock;
using Time = std::chrono::time_point<Clock, Seconds>;

class Timer {
public:
	void Start();
	Time Now();
	Seconds Elapsed();
	bool Past(Time);

private:
	Time startTime_;
};