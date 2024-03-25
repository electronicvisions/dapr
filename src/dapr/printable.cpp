#include "dapr/printable.h"

#include <ostream>

namespace dapr {

std::ostream& operator<<(std::ostream& os, Printable const& printable)
{
	return printable.print(os);
}

} // namespace dapr
