#include "gtest\gtest.h"
#include "ecs\ecs.h"

TEST(EntityManagerTest, EntityCreation) {
	ecs::EntityManager manager;
	manager.Create();
	EXPECT_EQ(1, manager.size());
	EXPECT_EQ(1, manager.capacity());
	manager.Create().Kill();
	EXPECT_EQ(1, manager.size());
	EXPECT_EQ(2, manager.capacity());
}