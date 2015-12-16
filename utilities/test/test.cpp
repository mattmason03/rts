#include "gtest\gtest.h"
#include "utilities\Timer.h"

double time_ = 0;
double getTime() {
	return time_;
}

class TimerTest : public testing::Test {
protected:
	Timer t;

	virtual void SetUp() {
		time_ = 0;
		Timer::SetTimeFunction(&getTime);
	}
};


TEST_F(TimerTest, Basic) {
	t.Start();
	EXPECT_EQ(0.0, t.Seconds());
	EXPECT_TRUE(t.Past(-1));
	EXPECT_FALSE(t.Past(1));

	time_ = 1;
	EXPECT_EQ(1.0, t.Seconds());
	EXPECT_TRUE(t.Past(0));
}