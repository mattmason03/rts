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
	auto e = manager.Create();
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
	auto e = manager.Create();
	EXPECT_EQ(1, manager.size());
	EXPECT_TRUE(e.Valid());
	e.Kill();
	EXPECT_EQ(0, manager.size());
	EXPECT_FALSE(e.Valid());
}


TEST_F(EntityManagerTest, EntityVersion) {
	auto e = manager.Create();
	EXPECT_EQ(0, e.GetId().version());
	EXPECT_EQ(0, e.GetId().index());
	e.Kill();
	e = manager.Create();
	EXPECT_EQ(1, e.GetId().version());
	EXPECT_EQ(0, e.GetId().index());
}

TEST_F(EntityManagerTest, ComponentAddition) {
	auto e = manager.Create()
		.Add<char>('a');
	EXPECT_TRUE(e.Has<char>());
	EXPECT_EQ(e.Get<char>(), 'a');
}

TEST_F(EntityManagerTest, ComponentDeletion) {
	auto e = manager.Create()
		.Add<char>('a')
		.Remove<char>();
	EXPECT_FALSE(e.Has<char>());
}

TEST_F(EntityManagerTest, ComponentModification) {
	auto e = manager.Create()
		.Add<char>('a');
	e.Get<char>() = 'e';
	EXPECT_EQ(e.Get<char>(), 'e');
}

TEST_F(EntityManagerTest, ComponentOverwrite) {
	auto e = manager.Create()
		.Add<char>('a')
		.Add<char>('e');
	EXPECT_EQ(e.Get<char>(), 'e');
}

TEST_F(EntityManagerTest, ComponentSimpleIteration) {
	auto e = manager.Create();

	manager.ForAll([&e](ecs::Entity::Id& id) {
		EXPECT_EQ(e.GetId(), id);
	});
}

TEST_F(EntityManagerTest, ComponentFilter) {
	manager.Create();

	manager.Create()
		.Add<char>('a')
		.Add<int>(4);

	manager.Create()
		.Add<char>('b');

	int count = 0;
	manager.ForAll([&count](ecs::Entity::Id& id) {
		count++;
	});
	EXPECT_EQ(count, 3);

	count = 0;

	manager.ForAll([&count](char& c) {
		count++;
	});
	EXPECT_EQ(count, 2);

	count = 0;
	manager.ForAll([&count](char& c, int& i) {
		count++;
	});
	EXPECT_EQ(count, 1);
}


