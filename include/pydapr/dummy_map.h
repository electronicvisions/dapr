#pragma once
#include "dapr/map.h"
#include "hate/visibility.h"
#include "pydapr/dummy_property.h"
#include "pydapr/genpybind.h"

namespace pydapr GENPYBIND_TAG_PYDAPR {

/**
 * DummyMapParent needed since direct typedefs in module not possible with current
 * genpybind.
 */
struct GENPYBIND(visible) DummyMapParent
{
	typedef dapr::Map<int, DummyProperty> PolymorphicValue GENPYBIND(opaque(false));

	typedef dapr::Map<int, int> NotPolymorphic GENPYBIND(opaque(false));
};

} // namespace pydapr
