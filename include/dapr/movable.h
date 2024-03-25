#pragma once
#include "dapr/genpybind.h"
#include "hate/visibility.h"
#include <memory>

namespace dapr {

/**
 * Object which is movable.
 * @tparam T Derived class
 */
template <typename T>
struct SYMBOL_VISIBLE Movable
{
	/**
	 * Move object.
	 */
	virtual std::unique_ptr<T> move() GENPYBIND(hidden) = 0;
};

} // namespace dapr
