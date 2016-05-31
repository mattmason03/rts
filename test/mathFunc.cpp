#include "glm\glm.hpp"
#include "glm\gtx\quaternion.hpp"
#include "glm\gtx\vector_angle.hpp"s
#include "gtest\gtest.h"
#include <iostream>

class MathFunc : public testing::Test {
protected:

	virtual void TearDown() {
	}

	static void TearDownTestCase() {
	}
};

TEST_F(MathFunc, dir) {
	float angle = 1.2f;
	auto quatAngle = glm::angleAxis(angle, glm::vec3(0, 0, 1));
	for (int i = 0; i < 10; i++) {
		
	}
}