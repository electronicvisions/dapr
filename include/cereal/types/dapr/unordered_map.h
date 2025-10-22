#pragma once
#include "dapr/unordered_map.h"

#include "cereal/types/dapr/property_holder.h"
#include <cereal/types/unordered_map.hpp>

namespace dapr {

template <
    typename KeyT,
    typename ValueT,
    template <typename...>
    typename KeyHolderT,
    template <typename...>
    typename ValueHolderT>
template <typename Archive>
void UnorderedMap<KeyT, ValueT, KeyHolderT, ValueHolderT>::serialize(Archive& ar, std::uint32_t)
{
	ar(m_values);
}

} // namespace dapr
