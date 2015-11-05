#include "gtest\gtest.h"
#include "ecs\ecs.h"
#include <iostream>

class EntityManagerTest : public testing::Test {
protected:
	ecs::EntityManager manager;

	virtual void TearDown() {
		manager.Reset();
	}
};


TEST_F(EntityManagerTest, EntityCreation) {
	manager.Create();
	EXPECT_EQ(1, manager.size());
	EXPECT_EQ(1, manager.capacity());
	manager.Create().Kill();
	EXPECT_EQ(1, manager.size());
	EXPECT_EQ(2, manager.capacity());

	for (int i = 0; i < 30; i++) {
		manager.Create().Kill();
		manager.Create().Add<char>((char)(i + 50));
	}
	manager.ForEach<ecs::Entity::Id, char>([](ecs::Entity::Id *id, char *c) {
		std::cout << id->index() << " : " << id->version() << " : " << *c << std::endl;
	});

	char x;
	std::cin >> x;
}