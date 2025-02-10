#pragma once
#include <stdexcept>
#include <utility>

namespace dapr::detail {

template <typename Key, typename Value>
struct UnorderedMapTransform
{
	template <typename U>
	constexpr std::pair<Key const&, Value const&> operator()(U&& value) const
	{
		if (!value.second) {
			throw std::logic_error("Unexpected access to moved-from object.");
		}
		return std::pair<Key const&, Value const&>{value.first, *value.second};
	}
};

} // namespace dapr
