#pragma once
#include "dapr/unordered_map.h"
#include "hate/visibility.h"
#include "pydapr/dummy_property.h"
#include "pydapr/genpybind.h"

namespace pydapr GENPYBIND_TAG_PYDAPR {

/**
 * DummyUnorderedMapParent needed since direct typedefs in module not possible with current
 * genpybind.
 */
struct GENPYBIND(visible) DummyUnorderedMapParent
{
	typedef dapr::UnorderedMap<int, DummyProperty> PolymorphicValue GENPYBIND(opaque(false));

	typedef dapr::UnorderedMap<DummyProperty, int> PolymorphicKey GENPYBIND(opaque(false));

	typedef dapr::UnorderedMap<DummyProperty, DummyProperty> PolymorphicKeyValue
	    GENPYBIND(opaque(false));

	typedef dapr::UnorderedMap<int, int> NotPolymorphic GENPYBIND(opaque(false));
};

} // namespace pydapr
