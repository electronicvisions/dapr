#pragma once
#include "dapr/equality_comparable.h"

#include <typeinfo>

namespace dapr {

template <typename T>
bool EqualityComparable<T>::operator==(T const& other) const
{
	if (typeid(*this) != typeid(other)) {
		return false;
	}
	return is_equal_to(static_cast<T const&>(other));
}

template <typename T>
bool EqualityComparable<T>::operator!=(T const& other) const
{
	return !operator==(other);
}

} // namespace dapr
