#pragma once

#include <iterator>

template <typename T>
class AnyIterator : public std::iterator<std::forward_iterator_tag, T> {
	virtual AnyIterator<T> operator++
};

template <typename T, typename Container>
class AnyContainerIterator : public AnyIterator<T>  {
	
};