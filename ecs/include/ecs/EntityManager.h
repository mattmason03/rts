#pragma once

#include <vector>
#include <bitset>

#include "Constants.h"
#include "Entity.h"
#include "Storage.h"

namespace ecs {
	class Storage;

	class EntityManager {
	public:
		EntityManager() {}

		Entity Create();
		void Kill(Entity::Id id);

		template <typename... Derived, typename F>
		void ForEach(F& f, Entity::Id start, Entity::Id end);
		template <typename... Derived, typename F>
		void ForEach(F& f, Entity::Id start);
		template <typename... Derived, typename F>
		void ForEach(F& f);

		template <typename Derived>
		void RegisterComponent();

		template <typename Derived, typename... DerivedArgs>
		void Add(Entity::Id id, DerivedArgs&&... args);

		template <typename Derived>
		Derived* Get(Entity::Id id);

		template <typename Derived>
		void Remove(Entity::Id id);

		size_t capacity();

		size_t size();

		typedef std::bitset<MAX_COMPONENTS> ComponentMask;

	private:
		template <typename Derived>
		ArrayStore* CreateOrGetStore();

		uint32_t indexCounter_ = 0;
		std::vector<ArrayStore*> componentStorage_;
		std::vector<uint32_t> entityFreeList_;
		std::vector<uint32_t> entityVersion_;
		std::vector<ComponentMask> entityMasks_;
	};

}

#include "EntityManagerImpl.h"