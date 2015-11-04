#include "gtest\gtest.h"
#include "ecs\ecs.h"

class EntityManagerTest : public testing::Test {
protected:
	ecs::EntityManager manager;
};


TEST_F(EntityManagerTest, EntityCreation) {
	manager.Create();
	EXPECT_EQ(1, manager.size());
	EXPECT_EQ(1, manager.capacity());
	manager.Create().Kill();
	EXPECT_EQ(1, manager.size());
	EXPECT_EQ(2, manager.capacity());
}