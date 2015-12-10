#include "utilities\Timer.h"
#include <cassert>

void Timer::Start() {
	assert(timeFunction_);
	startTime_ += timeFunction_() - stopTime_;
	stopTime_ = 0;
}

double Timer::Seconds() {
	assert(timeFunction_);
	return timeFunction_() - startTime_;
}

bool Timer::Past(double seconds) {
	assert(timeFunction_);
	return timeFunction_() > seconds + startTime_;
}

void Timer::Stop() {
	assert(timeFunction_);
	if (!stopTime_)
		stopTime_ = timeFunction_();
}

void Timer::Reset() {
	assert(timeFunction_);
	startTime_ = 0;
	stopTime_ = 0;
}

Timer::FPtr Timer::timeFunction_ = nullptr;

void Timer::SetTimeFunction(Timer::FPtr timeFunction) {
	timeFunction_ = timeFunction;
}