#pragma once

#if defined(__has_include) && !defined(PYPLUSPLUS)
#if __has_include(<genpybind.h>)
#include <genpybind.h>
#else
#define GENPYBIND(...)
#define GENPYBIND_MANUAL(...)
#endif
#else
#define GENPYBIND(...)
#define GENPYBIND_MANUAL(...)
#endif
