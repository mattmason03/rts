#pragma once

namespace ecs {
	class EntityManager;

	class Entity {
	public:
		struct Id {
			Id() : id_(0) {}
			explicit Id(uint64_t id) : id_(id) {}
			Id(uint32_t index, uint32_t version) : id_(uint64_t(index) | uint64_t(index) << 32UL) {}

			bool operator == (const Id &other) const { return id_ == other.id_; }
			bool operator != (const Id &other) const { return id_ != other.id_; }
			bool operator < (const Id &other) const { return id_ < other.id_; }

			uint64_t id() const { return id_; }
			uint32_t index() const { return id_ & 0xffffffffUL; }
			uint32_t version() const { return id_ >> 32; }

		private:
			uint64_t id_;
		};

		Entity(EntityManager* manager, Entity::Id id) : manager_(manager), id_(id) {}
		Entity(const Entity &other) = default;
		Entity &operator = (const Entity &other) = default;

		Entity::Id GetId();

		void Kill();

		template <typename Derived, typename... DerivedArgs>
		void Add(DerivedArgs&&... args);

		template <typename Derived>
		Derived* Get();

		template <typename Derived>
		void Remove();

	private:
		Entity::Id id_;
		EntityManager* manager_;
	};
}

#include "EntityImpl.h"