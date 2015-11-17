#pragma once

#include "PositionTransform.h"

struct PositionCurve {
	PositionCurve(PositionTransform prev, PositionTransform next) : prev(prev), next(next) {}
	PositionTransform prev;
	PositionTransform next;
};