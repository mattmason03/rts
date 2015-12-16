#pragma once

class Timer {
public:
	void Start();
	double Seconds();
	bool Past(double seconds);

	typedef double(*FPtr)();
	static void SetTimeFunction(FPtr timeFunction);

private:
	static FPtr timeFunction_;
	double startTime_ = 0;
};