#pragma once

#include <vector>

namespace ecs {
	class ArrayStore {
	public:
		ArrayStore(size_t chunkSize, size_t eltSize) : chunkSize_(chunkSize), eltSize_(eltSize), eltPerChunk_(chunkSize / eltSize) {}

		char* CreateOrGet(uint32_t index);

		char* Get(uint32_t index);

	protected:
		size_t GetChunkIndex(uint32_t index);
		size_t GetOffset(uint32_t index);
		std::vector<std::unique_ptr<char[]>> memory_;
		const size_t eltPerChunk_;
		const size_t eltSize_;
		const size_t chunkSize_;
	};

	template <typename Derived>
	class TypedArrayStore : public ArrayStore {
	public:
		TypedArrayStore() : ArrayStore(8192, sizeof(Derived)) {}
	};

	size_t ArrayStore::GetChunkIndex(uint32_t index) {
		return index / eltPerChunk_;
	}

	size_t ArrayStore::GetOffset(uint32_t index) {
		return index % eltPerChunk_ * eltSize_;
	}

	char* ArrayStore::CreateOrGet(uint32_t index) {
		size_t idx = GetChunkIndex(index);
		if (memory_.size() <= idx) {
			memory_.resize(idx + 1);
		}
		if (!memory_[idx]) {
			memory_[idx].reset(new char[chunkSize_]);
		}
		return memory_[idx].get() + GetOffset(index);
	}

	char* ArrayStore::Get(uint32_t index) {
		return memory_[GetChunkIndex(index)].get() + GetOffset(index);
	}
}