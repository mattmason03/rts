#pragma once

class Timer {
public:
	void Start();
	double Seconds();
	bool Past(double seconds);
	void Stop();
	void Reset();

	typedef double(*FPtr)();
	static void SetTimeFunction(FPtr timeFunction);

private:
	static FPtr timeFunction_;
	double startTime_ = 0;
	double stopTime_ = 0;
};