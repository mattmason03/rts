#pragma once

#include <cassert>

#include "Constants.h"

namespace ecs {
	class ComponentRegister {
	protected:
		static size_t componentCount;
	};

	template <typename Derived>
	class Component : ComponentRegister {
	public:
		static size_t Id();
	};

	size_t ComponentRegister::componentCount = 0;

	template <typename Derived>
	size_t Component<Derived>::Id() {
		// static function member only initialized once
		static size_t id = ComponentRegister::componentCount++;
		assert(id < MAX_COMPONENTS);
		return id;
	}

	size_t Component<Entity>::Id() {
		return 0;
	}

}