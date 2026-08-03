#pragma once
#include "dapr/empty_property.h"
#include "dapr/hashable.h"
#include "hate/visibility.h"
#include <cstddef>
#include <iosfwd>
#include <memory>
#include <type_traits>

namespace dapr {

/**
 * Hashable property type without content for using when the type information alone suffices.
 */
template <typename Derived, typename Base>
struct SYMBOL_VISIBLE EmptyHashableProperty : public EmptyProperty<Derived, Base>
{
	virtual size_t hash() const override;

private:
	static_assert(
	    std::is_base_of_v<Hashable, Base>,
	    "EmptyHashableProperty expects Base to be derived from Hashable.");
};

} // namespace dapr

#include "dapr/empty_hashable_property.tcc"
