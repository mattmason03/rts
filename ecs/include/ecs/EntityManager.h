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

		bool Valid(Entity::Id id);

		// Kill entity
		void Kill(Entity::Id id);

		// For each entity that has Derived... components, runs the provided function
		// Function should have pointer arguments of *Derived
		// i.e. manager.ForEach<Entity::Id, Position>([&manager](Entity::Id *id, Position *pos) { pos.x += 1; manager.Kill(*id); });

		template <typename F>
		void ForEach(F f);

		template <typename... Derived, typename F>
		void ForEach_(F& f, Entity::Id start, Entity::Id end);
		template <typename... Derived, typename F>
		void ForEach_(F& f, Entity::Id start);
		template <typename... Derived, typename F>
		void ForEach_(F& f);

		// Add Derived component to entity, passing DerivedArgs... to Derived constructor
		// i.e. mananger.Add<Position>(entityId, xPos, yPos);
		template <typename Derived, typename... DerivedArgs>
		void Add(Entity::Id id, DerivedArgs&&... args);

		// Get Derived component from entity
		template <typename Derived>
		Derived* Get(Entity::Id id);

		// Test if entity has component
		template <typename Derived>
		bool Has(Entity::Id id);

		// Remove Derived component from entity
		template <typename Derived>
		void Remove(Entity::Id id);

		// Remove all entities, capacity stays same
		void Reset();

		// Total number of supported entities
		size_t capacity();

		// Total number of live entities
		size_t size();

		typedef std::bitset<MAX_COMPONENTS> ComponentMask;

	private:
		template <typename Derived>
		ArrayStore* CreateOrGetStore();

		uint32_t indexCounter_ = 0;
		std::vector<std::unique_ptr<ArrayStore>> componentStorage_;
		std::vector<uint32_t> entityFreeList_;
		std::vector<uint32_t> entityVersion_;
		std::vector<ComponentMask> entityMasks_;
	};

}

#include "EntityManagerImpl.h"