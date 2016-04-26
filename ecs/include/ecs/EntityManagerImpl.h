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

	template <typename... Derived, typename F>
	void EntityManager::ForEach_(F& f, Entity::Id first, Entity::Id last) {
		ComponentMask mask = Unpacker<Derived...>::BuildComponentMask();
		for (uint32_t size = capacity(), start = first.index(), end = last.index(); start != end && start < size; ++start) {
			if ((mask & entityMasks_[start]) == mask) {
				Unpacker<Derived...>::Call(f, start, componentStorage_);
			}
		}
	}

	template <typename... Derived, typename F>
	void EntityManager::ForEach_(F& f, Entity::Id start) {
		ForEach_<Derived...>(f, start, capacity());
	}

	template <typename... Derived, typename F>
	void EntityManager::ForEach_(F& f) {
		ForEach_<Derived...>(f, 0, capacity());
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

	template <typename Derived, typename... DerivedArgs>
	void EntityManager::Add(Entity::Id id, DerivedArgs&&... args) {
		assert(Valid(id));
		uint32_t idx = id.index();
		entityMasks_[idx].set(Component<Derived>::Id());
		ArrayStore* store = CreateOrGetStore<Derived>();
		new(store->CreateOrGet(idx)) Derived(std::forward<DerivedArgs>(args)...);
	}

	template <typename Derived>
	Derived* EntityManager::Get(Entity::Id id) {
		assert(Has<Derived>(id));
		return (Derived*)componentStorage_[Component<Derived>::Id()]->Get(id.index());
	}

	template <typename Derived>
	bool EntityManager::Has(Entity::Id id) {
		assert(Valid(id));
		return entityMasks_[id.index()].test(Component<Derived>::Id());
	}

	template <typename Derived>
	void EntityManager::Remove(Entity::Id id) {
		assert(Valid(id));
		entityMasks_[id.index()].reset(Component<Derived>::Id());
	}

	void EntityManager::Reset() {
		entityFreeList_.clear();
		for (size_t i = 0; i < capacity(); i++) {
			entityMasks_[i].reset();
			entityVersion_[i] = 0;
			entityFreeList_.push_back(i);
		}
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
			componentStorage_.resize(componentId + 1);
		}
		if (!componentStorage_[componentId])
			componentStorage_[componentId].reset(new TypedArrayStore<Derived>());
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
		static void Call(Function& function, size_t entityIndex, std::vector<std::unique_ptr<ArrayStore>>& storage, UnpackedArgs... unpackedArgs) {
			// Call function with all unpacked arguments
			function(unpackedArgs...);
		}
	};

	// Pop off top argument and process
	template <typename CurrentArg, typename... Arguments>
	struct Unpacker < CurrentArg, Arguments... >
	{
	public:
		using curArg = typename std::remove_pointer<CurrentArg>::type;
		static EntityManager::ComponentMask BuildComponentMask() {
			// Build a mask from remaining arguments
			EntityManager::ComponentMask mask = Unpacker<Arguments...>::BuildComponentMask();
			// Set the mask for current argument
			mask.set(Component<curArg>::Id());
			return mask;
		}

		template <typename Function, typename... UnpackedArgs>
		static void Call(Function& function, size_t entityIndex, std::vector<std::unique_ptr<ArrayStore>>& storage, UnpackedArgs... unpackedArgs) {
			// Get array store for current component type
			// Retrieve component for specified entity
			curArg* component = (curArg*)(storage[Component<curArg>::Id()]->Get(entityIndex));
			// Add unpacked component argument and recurse
			Unpacker<Arguments...>::Call(function, entityIndex, storage, unpackedArgs..., component);
		}
	};
}