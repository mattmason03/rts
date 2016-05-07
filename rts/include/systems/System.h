#pragma once

#include "TimeTypes.h"

class System {
public:
	virtual void Register() {};

	virtual void Update(Duration& timestep) {};
};