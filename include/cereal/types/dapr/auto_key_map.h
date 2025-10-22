#pragma once
#include "dapr/auto_key_map.h"

#include "cereal/types/dapr/map.h"

namespace dapr {

template <typename KeyT, typename ValueT, typename BackendT>
template <typename Archive>
void AutoKeyMap<KeyT, ValueT, BackendT>::serialize(Archive& ar, std::uint32_t)
{
	ar(m_values);
	ar(m_next_key);
}

} // namespace dapr
