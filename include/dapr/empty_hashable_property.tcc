#pragma once
#include "dapr/empty_hashable_property.h"

#include "hate/type_index.h"

namespace dapr {

template <typename Derived, typename Base>
size_t EmptyHashableProperty<Derived, Base>::hash() const
{
	return std::hash<std::string>()(hate::full_name<Derived>());
}

} // namespace dapr
