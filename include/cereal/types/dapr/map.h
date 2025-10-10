#pragma once
#include "dapr/map.h"

#include "cereal/types/dapr/property_holder.h"
#include <cereal/types/map.hpp>

namespace dapr {

template <typename KeyT, typename ValueT>
template <typename Archive>
void Map<KeyT, ValueT>::serialize(Archive& ar, std::uint32_t)
{
	ar(m_values);
}

} // namespace dapr
