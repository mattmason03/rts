#pragma once

#include "EntityManager.h"
#include "TypeRegistry.h"
#include <typeinfo>
#include <iostream>

namespace ecs {
	Entity EntityManager::Create() {
		uint32_t index;
		if (entityFreeList_.size() > 0) {
			index = entityFreeList_.back();
			entityFreeList_.pop_back();
		}
		else {
			index = indexCounter_++;
			entityMasks_.emplace_back();
			entityVersion_.emplace_back(0);
		}
		Entity::Id id(index, entityVersion_[index]);

		Add<Entity::Id>(id, id);
		return Entity(this, id);
	}

	
	template <size_t idx, typename Tuple, typename... Components>
	struct TuplePack;

	template <size_t idx, typename Tuple, typename Component, typename... Components>
	struct TuplePack<idx, Tuple, Component, Components...> {
		static void AddComponent(Entity& e, const Tuple& t) {
			e.Add<Component>(std::get<idx>(t));
			TuplePack<idx + 1, Tuple, Components...>::AddComponent(e, t);
		}
	};

	template <size_t idx, typename Tuple, typename Component>
	struct TuplePack<idx, Tuple, Component> {
		static void AddComponent(Entity& e, const Tuple& t) {
			e.Add<Component>(std::get<idx>(t));
		}
	};

	template <typename... Component>
	Entity EntityManager::Create(std::tuple<Component...>& t) {
		auto e = Create();
		TuplePack<0, std::tuple<Component...>, Component...>::AddComponent(e, t);
		return e;
	}

	bool EntityManager::Valid(Entity::Id id) {
		uint32_t index = id.index();
		return entityVersion_[index] == id.version();
	}

	void EntityManager::Kill(Entity::Id id) {
		assert(Valid(id));
		entityFreeList_.push_back(id.index());
		entityVersion_[id.index()]++;
		entityMasks_[id.index()].reset();
	}

	template <typename... Component, typename F>
	void EntityManager::ForEach_(F& f, Entity::Id first, Entity::Id last) {
		ComponentMask mask = Unpacker<Component...>::BuildComponentMask();
		for (uint32_t size = capacity(), start = first.index(), end = last.index(); start != end && start < size; ++start) {
			if ((mask & entityMasks_[start]) == mask) {
				Unpacker<Component...>::Call(this, f, start, componentStorage_);
			}
		}
	}

	template <typename... Component, typename F>
	void EntityManager::ForEach_(F& f, Entity::Id start) {
		ForEach_<Component...>(f, start, capacity());
	}

	template <typename... Component, typename F>
	void EntityManager::ForEach_(F& f) {
		ForEach_<Component...>(f, 0, capacity());
	}

	template <typename T>
	struct f_trait
		: public f_trait<decltype(&T::operator())>
	{};

	template <typename C, typename R, typename... Args>
	struct f_trait<R(C::*)(Args...) const>
	{
		template <typename T>
		void Unwrap(EntityManager* manager, T f) {
			manager->ForEach_<Args...>(f);
		}
	};

	template <typename F>
	void EntityManager::ForAll(F f) {
		f_trait<F> trait;
		trait.Unwrap(this, f);
	}

	template <typename Component, typename... ComponentArgs>
	void EntityManager::Add(Entity::Id id, ComponentArgs&&... args) {
		assert(Valid(id));
		uint32_t idx = id.index();
		entityMasks_[idx].set(TypeRegistry<Component>::Id());
		ArrayStore* store = CreateOrGetStore<Component>();
		new(store->CreateOrGet(idx)) Component(std::forward<ComponentArgs>(args)...);
	}

	template <typename Component>
	Component& EntityManager::Get(Entity::Id id) {
		assert(Has<Component>(id));
		return *reinterpret_cast<Component*>(componentStorage_[TypeRegistry<Component>::Id()]->Get(id.index()));
	}

	template <typename Component>
	bool EntityManager::Has(Entity::Id id) {
		assert(Valid(id));
		return entityMasks_[id.index()].test(TypeRegistry<Component>::Id());
	}

	template <typename Component>
	void EntityManager::Remove(Entity::Id id) {
		assert(Valid(id));
		entityMasks_[id.index()].reset(TypeRegistry<Component>::Id());
	}

	void EntityManager::Reset() {
		entityFreeList_.clear();
		for (uint32_t i = 0; i < capacity(); i++) {
			entityMasks_[i].reset();
			entityVersion_[i] = 0;
			entityFreeList_.push_back(i);
		}
	}

	uint32_t EntityManager::capacity() {
		return static_cast<uint32_t>(entityMasks_.size());
	}

	uint32_t EntityManager::size() {
		return capacity() - static_cast<uint32_t>(entityFreeList_.size());
	}

	template <typename Component>
	ArrayStore* EntityManager::CreateOrGetStore() {
		size_t componentId = TypeRegistry<Component>::Id();
		if (componentStorage_.size() <= componentId) {
			componentStorage_.resize(componentId + 1);
		}
		if (!componentStorage_[componentId])
			componentStorage_[componentId].reset(new TypedArrayStore<Component>());
		return componentStorage_[componentId].get();
	}

	template <typename... Arguments>
	struct Unpacker;

	// Base case of template recursion
	template <>
	struct Unpacker < >
	{
	public:
		static EntityManager::ComponentMask BuildComponentMask() {
			// Return empty mask
			return EntityManager::ComponentMask();
		}

		template <typename Function, typename... UnpackedArgs>
		static void Call(EntityManager * manager, Function& function, uint32_t entityIndex, std::vector<std::unique_ptr<ArrayStore>>& storage, UnpackedArgs... unpackedArgs) {
			// Call function with all unpacked arguments
			function(*unpackedArgs...);
		}
	};

	// Pop off top argument and process
	template <typename CurrentArg, typename... Arguments>
	struct Unpacker < CurrentArg, Arguments... >
	{
	public:
		using requestedType = typename std::remove_reference<CurrentArg>::type;
		static EntityManager::ComponentMask BuildComponentMask() {
			// Build a mask from remaining arguments
			EntityManager::ComponentMask mask = Unpacker<Arguments...>::BuildComponentMask();
			// Set the mask for current argument
			mask.set(TypeRegistry<requestedType>::Id());
			return mask;
		}

		template <typename Function, typename... UnpackedArgs>
		static void Call(EntityManager * manager, Function& function, uint32_t entityIndex, std::vector<std::unique_ptr<ArrayStore>>& storage, UnpackedArgs... unpackedArgs) {
			// Get array store for current component type
			// Retrieve component for specified entity
			requestedType* component;
			const auto isEntity = std::is_same<requestedType, Entity>();
			if (isEntity) {
				// needs cast to satisfy compiler, lol
				auto e = new Entity(manager, *(Entity::Id*)storage[TypeRegistry<Entity::Id>::Id()]->Get(entityIndex));
				component = reinterpret_cast<requestedType*>(e);
			}
			else {
				component = reinterpret_cast<requestedType*>((storage[TypeRegistry<requestedType>::Id()]->Get(entityIndex)));
			}

			//const requestedType& = [this]() {
			//	const auto isEntity = std::is_same<requestedType, Entity>();
			//	if (isEntity) {
			//		// needs cast to satisfy compiler, lol
			//		return reinterpret_cast<requestedType*>(new Entity(manager, *(Entity::Id*)storage[0]->Get(entityIndex)));
			//	}
			//	else {
			//		return reinterpret_cast<requestedType*>((storage[TypeRegistry<requestedType>::Id()]->Get(entityIndex)));
			//	}
			//}();
			
			// Add unpacked component argument and recurse
			Unpacker<Arguments...>::Call(manager, function, entityIndex, storage, unpackedArgs..., component);
			if (isEntity) {
				delete component;
			}
		}
	};
}