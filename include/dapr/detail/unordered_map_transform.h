#pragma once
#include "dapr/property_holder.h"
#include <stdexcept>
#include <utility>

namespace dapr::detail {

template <
    typename Key,
    typename Value,
    template <typename...>
    typename ValueHolder,
    template <typename...>
    typename KeyHolder>
struct UnorderedMapTransform
{
	template <typename U>
	constexpr std::pair<Key const&, Value const&> operator()(U&& value) const
	{
		if constexpr (std::is_same_v<
		                  std::remove_const_t<typename std::decay_t<U>::first_type>,
		                  PropertyHolder<Key, KeyHolder>>) {
			if constexpr (std::is_same_v<
			                  typename std::decay_t<U>::second_type,
			                  PropertyHolder<Value, ValueHolder>>) {
				return std::pair<Key const&, Value const&>{*value.first, *value.second};
			} else {
				return std::pair<Key const&, Value const&>{*value.first, value.second};
			}
		} else {
			if constexpr (std::is_same_v<
			                  typename std::decay_t<U>::second_type,
			                  PropertyHolder<Value, ValueHolder>>) {
				return std::pair<Key const&, Value const&>{value.first, *value.second};
			} else {
				return std::pair<Key const&, Value const&>{value.first, value.second};
			}
		}
	}
};

} // namespace dapr::detail
