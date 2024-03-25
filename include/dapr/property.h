#pragma once
#include "dapr/copyable.h"
#include "dapr/equality_comparable.h"
#include "dapr/movable.h"
#include "dapr/printable.h"

namespace dapr {

/**
 * Property, which we define to have the following properties:
 *  - copyable
 *  - movable
 *  - printable
 *  - equality-comparable
 */
template <typename Derived>
struct SYMBOL_VISIBLE Property
    : public Copyable<Derived>
    , public Movable<Derived>
    , public Printable
    , public EqualityComparable<Derived>
{};

} // namespace dapr
