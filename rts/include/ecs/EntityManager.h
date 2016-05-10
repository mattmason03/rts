#pragma once

#include <vector>
#include <bitset>
#include <memory>
#include <functional>

#include "Constants.h"
#include "Entity.h"
#include "Storage.h"

namespace ecs {
	class Storage;

	class EntityManager {
	public:
		EntityManager() {}

		// Create an entity
		Entity Create();

		template <typename... Component>
		Entity Create(std::tuple<Component...>& components);

		bool Valid(Entity::Id id);

		// Kill entity
		void Kill(Entity::Id id);

		// For each entity that has Component... components, runs the provided function
		// Function should have Component& args
		// i.e. manager.ForEach<Entity::Id, Position>([&manager](Entity::Id& id, Position& pos) { pos.x += 1; manager.Kill(*id); });

		template <typename F>
		void ForAll(F f);

		template <typename... Component, typename F>
		void ForEach_(F& f, Entity::Id start, Entity::Id end);
		template <typename... Component, typename F>
		void ForEach_(F& f, Entity::Id start);
		template <typename... Component, typename F>
		void ForEach_(F& f);

		// Add Component component to entity, passing ComponentArgs... to Component constructor
		// i.e. mananger.Add<Position>(entityId, xPos, yPos);
		template <typename Component, typename... ComponentArgs>
		void Add(Entity::Id id, ComponentArgs&&... args);

		// Get Component component from entity
		template <typename Component>
		Component& Get(Entity::Id id);

		// Test if entity has component
		template <typename Component>
		bool Has(Entity::Id id);

		// Remove Component component from entity
		template <typename Component>
		void Remove(Entity::Id id);

		// Remove all entities, capacity stays same
		void Reset();

		// Total number of supported entities
		uint32_t capacity();

		// Total number of live entities
		uint32_t size();

		typedef std::bitset<MAX_COMPONENTS> ComponentMask;

	private:
		template <typename Component>
		ArrayStore* CreateOrGetStore();

		uint32_t indexCounter_ = 0;
		std::vector<std::unique_ptr<ArrayStore>> componentStorage_;
		std::vector<uint32_t> entityFreeList_;
		std::vector<uint32_t> entityVersion_;
		std::vector<ComponentMask> entityMasks_;
	};

}

#include "EntityManagerImpl.h"