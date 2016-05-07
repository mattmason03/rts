#pragma once

#include <chrono>

using Clock = std::chrono::high_resolution_clock;
using Time = Clock::time_point;
using Duration = Clock::duration;
using Millis = std::chrono::milliseconds;
using Micros = std::chrono::microseconds;
