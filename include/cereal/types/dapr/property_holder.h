#pragma once
#include "dapr/property_holder.h"

#include <cereal/types/memory.hpp>

namespace dapr {

template <typename T, template <typename...> typename Backend>
template <typename Archive>
void PropertyHolder<T, Backend>::serialize(Archive& ar, std::uint32_t)
{
	ar(m_backend);
}

} // namespace dapr
