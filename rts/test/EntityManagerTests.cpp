#include "gtest\gtest.h"
#include "ecs\ecs.h"
#include <iostream>

class EntityManagerTest : public testing::Test {
protected:
	ecs::EntityManager manager;

	virtual void TearDown() {
		manager.Reset();
	}

	static void TearDownTestCase() {
	}
};

TEST_F(EntityManagerTest, EntityCreation) {
	ecs::Entity e = manager.Create();
	EXPECT_EQ(0, e.GetId().index());
	EXPECT_EQ(0, e.GetId().version());
	EXPECT_EQ(1, manager.size());
	EXPECT_EQ(1, manager.capacity());

	e = manager.Create();
	EXPECT_EQ(1, e.GetId().index());
	EXPECT_EQ(0, e.GetId().version());
	EXPECT_EQ(2, manager.size());
	EXPECT_EQ(2, manager.capacity());
}

TEST_F(EntityManagerTest, EntityKill) {
	ecs::Entity e = manager.Create();
	EXPECT_EQ(1, manager.size());
	EXPECT_TRUE(e.Valid());
	e.Kill();
	EXPECT_EQ(0, manager.size());
	EXPECT_FALSE(e.Valid());
}


TEST_F(EntityManagerTest, EntityVersion) {
	ecs::Entity e = manager.Create();
	EXPECT_EQ(0, e.GetId().version());
	EXPECT_EQ(0, e.GetId().index());
	e.Kill();
	e = manager.Create();
	EXPECT_EQ(1, e.GetId().version());
	EXPECT_EQ(0, e.GetId().index());
}

TEST_F(EntityManagerTest, ComponentAddition) {
	ecs::Entity e = manager.Create();
	e.Add<char>('a');
	EXPECT_TRUE(e.Has<char>());
	EXPECT_EQ(*e.Get<char>(), 'a');
}

TEST_F(EntityManagerTest, ComponentDeletion) {
	ecs::Entity e = manager.Create();
	e.Add<char>('a');
	e.Remove<char>();
	EXPECT_FALSE(e.Has<char>());
}

TEST_F(EntityManagerTest, ComponentModification) {
	ecs::Entity e = manager.Create();
	e.Add<char>('a');
	*e.Get<char>() = 'e';
	EXPECT_EQ(*e.Get<char>(), 'e');
}

TEST_F(EntityManagerTest, ComponentOverwrite) {
	ecs::Entity e = manager.Create();
	e.Add<char>('a');
	e.Add<char>('e');
	EXPECT_EQ(*e.Get<char>(), 'e');
}

TEST_F(EntityManagerTest, ComponentSimpleIteration) {
	ecs::Entity e = manager.Create();

	manager.ForEach_<ecs::Entity::Id*>([&](ecs::Entity::Id *id) {
		EXPECT_EQ(e.GetId(), *id);
	});
}

void test(ecs::Entity::Id *id) {
	int x = 4;
}

TEST_F(EntityManagerTest, ComponentFilter) {
	manager.Create();

	ecs::Entity e = manager.Create();
	e.Add<char>('a');
	e.Add<int>(4);

	e = manager.Create();
	e.Add<char>('b');

	int count = 0;
	manager.ForAll([&](ecs::Entity::Id *id) {
		count++;
	});
	//manager.ForEach<ecs::Entity::Id>([&](ecs::Entity::Id *id) {
	//	count++;
	//});
	EXPECT_EQ(count, 3);

	count = 0;


	manager.ForEach_<char*>([&](char* c) {
		count++;
	});
	EXPECT_EQ(count, 2);

	count = 0;
	manager.ForEach_<char*, int*>([&](char* c, int* i) {
		count++;
	});
	EXPECT_EQ(count, 1);
}


