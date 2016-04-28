#pragma once
#include <memory>
#include <functional>

template<typename T>
class WrapMemberFunctions {
	typedef void (T::*member)();

private:
	T* p_;
	member prefix_, suffix_;

public:

	explicit WrapMemberFunctions(T* p, member prefix, member suffix) : p_(p), prefix_(prefix), suffix_(suffix) {	}

	std::shared_ptr<T> operator->() const {
		std::mem_fn(prefix_)(p_);
		return std::shared_ptr<T>(p_, std::mem_fn(suffix_));
	}
};