#include "gtest\gtest.h"
#include "utilities\WrapMembers.h"

class ABC {
public:
	std::string value;

	void addA() {
		value += 'a';
	}

	void addB() {
		value += 'b';
	}

	void addC() {
		value += 'c';
	}
};

class WrapFunctionTest : public testing::Test {
protected:
	ABC abc;

	virtual void SetUp() {
		abc.value = "";
	}
};


TEST_F(WrapFunctionTest, No_Wrap) {
	abc.addA();
	EXPECT_EQ("a", abc.value);
	abc.addB();
	EXPECT_EQ("ab", abc.value);
	abc.addC();
	EXPECT_EQ("abc", abc.value);
}

TEST_F(WrapFunctionTest, Wrap) {
	WrapMemberFunctions<ABC> pABC(&abc, &ABC::addA, &ABC::addC);
	pABC->addB();
	EXPECT_EQ("abc", abc.value);
	pABC->addA();
	EXPECT_EQ("abcaac", abc.value);
}