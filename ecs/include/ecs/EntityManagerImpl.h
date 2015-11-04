#pragma once

#include "EntityManager.h"
#include "Component.h"

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

	void EntityManager::Kill(Entity::Id id) {
		entityFreeList_.push_back(id.index());
		entityVersion_[id.index()]++;
		entityMasks_[id.index()].reset();
	}

	template <typename... Derived, typename F>
	void EntityManager::ForEach(F& f, Entity::Id start, Entity::Id end) {
		ComponentMask mask = Unpacker<Derived...>::BuildComponentMask();
		std::vector<ArrayStore*> pools = Unpacker<Derived...>::GetPools(*this);
		for (size_t size = capacity(); start != end && start < size; ++start) {
			if ((mask & entityMasks_[start]) == mask) {
				Unpacker<Derived...>::Call(f, start, pools);
			}
		}
	}

	template <typename... Derived, typename F>
	void EntityManager::ForEach(F& f, Entity::Id start) {
		ForEach(f, start, capacity());
	}

	template <typename... Derived, typename F>
	void EntityManager::ForEach(F& f) {
		ForEach(f, 0, capacity());
	}

	template <typename >
	void EntityManager::RegisterComponent() {
		Derived::Register();
		componentStorage_[Component<Derived>::Id()] = new TypedArrayStore < Derived >();
	}

	template <typename Derived, typename... DerivedArgs>
	void EntityManager::Add(Entity::Id id, DerivedArgs&&... args) {
		uint32_t idx = id.index();
		entityMasks_[idx].set(Component<Derived>::Id());
		ArrayStore* store = CreateOrGetStore<Derived>();
		new(store->CreateOrGet(idx)) Derived(std::forward<DerivedArgs>(args)...);
	}

	template <typename Derived>
	Derived* EntityManager::Get(Entity::Id id) {
		return (Derived*)componentStorage_[Component<Derived>::Id()]->Get(id);
	}

	template <typename Derived>
	void EntityManager::Remove(Entity::Id id) {
		entityMasks_[id].reset(Component<Derived>::Id());
	}

	size_t EntityManager::capacity() {
		return entityMasks_.size();
	}

	size_t EntityManager::size() {
		return capacity() - entityFreeList_.size();
	}

	template <typename Derived>
	ArrayStore* EntityManager::CreateOrGetStore() {
		size_t componentId = Component<Derived>::Id();
		if (componentStorage_.size() <= componentId) {
			componentStorage_.resize(componentId + 1, nullptr);
		}
		if (!componentStorage_[componentId]) {
			componentStorage_[componentId] = new TypedArrayStore<Derived>();
		}
		return componentStorage_[componentId];
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
		static void Call(Function& function, size_t entityIndex, std::vector<ArrayStore*>& storage, UnpackedArgs... unpackedArgs) {
			// Call function with all unpacked arguments
			function(unpackedArgs...);
		}
	};

	// Pop off top argument and process
	template <typename CurrentArg, typename... Arguments>
	struct Unpacker < CurrentArg, Arguments... >
	{
	public:
		static EntityManager::ComponentMask BuildComponentMask() {
			// Build a mask from remaining arguments
			EntityManager::ComponentMask mask = Unpacker<Arguments...>::BuildComponentMask();
			// Set the mask for current argument
			mask.set(Component<CurrentArg>::Id());
			return mask;
		}

		template <typename Function, typename... UnpackedArgs>
		static void Call(Function& function, size_t entityIndex, std::vector<ArrayStore*>& storage, UnpackedArgs... unpackedArgs) {
			// Get array store for current component type
			ArrayStore* arrayStore = storage[Component<CurrentArg>::Id()];
			// Retrieve component for specified entity
			CurrentArg* component = arrayStore->Get(entityIndex);
			// Add unpacked component argument and recurse
			Unpacker<Derived...>::Call(function, entityIndex, storage, unpackedArgs..., component);
		}
	};
}