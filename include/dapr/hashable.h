#pragma once
#include <cstddef>
#include <functional>

namespace dapr {

struct Hashable
{
	virtual size_t hash() const = 0;
};

} // namespace dapr

namespace std {

template <>
struct hash<dapr::Hashable>
{
	size_t operator()(dapr::Hashable const& value) const
	{
		return value.hash();
	}
};

} // namespace std
