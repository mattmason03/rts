#pragma once

#include "Entity.h"
#include "EntityManager.h"

namespace ecs {
	Entity::Id Entity::GetId() {
		return id_;
	}

	bool Entity::Valid() {
		return manager_->Valid(id_);
	}

	void Entity::Kill() {
		manager_->Kill(id_);
	}

	template <typename Derived, typename... DerivedArgs>
	void Entity::Add(DerivedArgs&&... args) {
		manager_->Add<Derived>(id_, args...);
	}

	template <typename Derived>
	Derived* Entity::Get() {
		return manager_->Get<Derived>(id_);
	}

	template <typename Derived>
	bool Entity::Has() {
		return manager_->Has<Derived>(id_);
	}

	template <typename Derived>
	void Entity::Remove() {
		manager_->Remove<Derived>(id_);
	}
}

