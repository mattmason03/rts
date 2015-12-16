#include "utilities\Timer.h"
#include <cassert>

void Timer::Start() {
	assert(timeFunction_);
	startTime_ += timeFunction_();
}

double Timer::Seconds() {
	assert(timeFunction_);
	return timeFunction_() - startTime_;
}

bool Timer::Past(double seconds) {
	assert(timeFunction_);
	return timeFunction_() > seconds + startTime_;
}

Timer::FPtr Timer::timeFunction_ = nullptr;

void Timer::SetTimeFunction(Timer::FPtr timeFunction) {
	timeFunction_ = timeFunction;
}