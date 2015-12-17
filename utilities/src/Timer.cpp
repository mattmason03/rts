#include "utilities\Timer.h"

void Timer::Start() {
	startTime_ = Clock::now();
}

Time Timer::Now() {
	return Clock::now();
}

Seconds Timer::Elapsed() {
	return Seconds(Now() - startTime_);
}

bool Timer::Past(Time t) {
	return Now() > t;
}
