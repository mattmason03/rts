#pragma once
#include <tuple>
#include <iterator>

template <typename T, typename... Iterables>
class ConcatIterables {
public:
	class iterator : public std::iterator<std::forward_iterator_tag, T> {
		size_t tupleIndex;
		
		T operator*() const {
			std::get<tupleIndex>(iterables_);
		}

	};

	ConcatIterables(Iterables&&... iterables) : iterables_(std::make_tuple(std::forward<Iterables>(iterables)...)) {}

	


private:
	std::tuple<Iterables...> iterables_;
};