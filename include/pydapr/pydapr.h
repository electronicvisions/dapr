#pragma once
#include "pydapr/dummy_map.h"
#include "pydapr/dummy_property.h"
#include "pydapr/dummy_unordered_map.h"
#include "pydapr/genpybind.h"

GENPYBIND_TAG_PYDAPR
GENPYBIND_MANUAL({ parent.attr("__variant__") = "pybind11"; })
