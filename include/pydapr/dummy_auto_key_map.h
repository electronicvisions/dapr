#pragma once
#include "dapr/auto_key_map.h"
#include "hate/visibility.h"
#include "pydapr/dummy_property.h"
#include "pydapr/genpybind.h"

namespace pydapr GENPYBIND_TAG_PYDAPR {

/**
 * DummyMapParent needed since direct typedefs in module not possible with current
 * genpybind.
 */
struct GENPYBIND(visible) DummyAutoKeyMapParent
{
	typedef dapr::AutoKeyMap<int, DummyProperty> PolymorphicValue GENPYBIND(opaque(false));

	typedef dapr::AutoKeyMap<int, int> NotPolymorphic GENPYBIND(opaque(false));
};

} // namespace pydapr
