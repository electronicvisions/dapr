#pragma once
#include "dapr/genpybind.h"
#include "hate/visibility.h"
#include <iosfwd>

namespace dapr {

/**
 * Object which is printable.
 */
struct SYMBOL_VISIBLE Printable
{
	/**
	 * Ostream operator for all objects which are derived from Printable.
	 */
	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, Printable const& printable) SYMBOL_VISIBLE;

protected:
	/**
	 * Print object.
	 * This method is to be implemented by derived classes.
	 */
	virtual std::ostream& print(std::ostream& os) const GENPYBIND(hidden) = 0;
};

} // namespace dapr
