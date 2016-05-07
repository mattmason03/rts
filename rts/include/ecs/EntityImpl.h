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

	template <typename Component, typename... ComponentArgs>
	Entity& Entity::Add(ComponentArgs&&... args) {
		manager_->Add<Component>(id_, args...);
		return *this;
	}

	template <typename Component>
	Component& Entity::Get() {
		return manager_->Get<Component>(id_);
	}

	template <typename Component>
	bool Entity::Has() {
		return manager_->Has<Component>(id_);
	}

	template <typename Component>
	Entity& Entity::Remove() {
		manager_->Remove<Component>(id_);
		return *this;
	}
}

