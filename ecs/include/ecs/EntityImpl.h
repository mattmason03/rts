#pragma once

#include "Entity.h"
#include "EntityManager.h"

namespace ecs {
	Entity::Id Entity::GetId() {
		return id_;
	}

	void Entity::Kill() {
		manager_->Kill(id_);
	}

	template <typename Derived, typename... DerivedArgs>
	void Entity::Add(DerivedArgs&&... args) {
		manager_->AddComponent<Derived>(id_, args...);
	}

	template <typename Derived>
	Derived* Entity::Get() {
		return manager_->GetComponent<Derived>(id_);
	}

	template <typename Derived>
	void Entity::Remove() {
		manager_->RemoveComponent<Derived>();
	}
}

