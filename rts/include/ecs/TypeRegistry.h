#pragma once

#include <cassert>

#include "Constants.h"

namespace ecs {
	class TypeCounter {
	protected:
		static size_t typeCount;
	};

	template <typename Component>
	class TypeRegistry : TypeCounter {
	public:
		static size_t Id();
	};

	size_t TypeCounter::typeCount = 0;

	template <typename Component>
	size_t TypeRegistry<Component>::Id() {
		// static function member only initialized once
		static size_t id = TypeCounter::typeCount++;
		assert(id < MAX_COMPONENTS);
		return id;
	}

	size_t TypeRegistry<Entity>::Id() {
		return 0;
	}

}