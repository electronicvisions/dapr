#include "pydapr/dummy_property.h"

namespace pydapr {

DummyProperty::~DummyProperty() {}


DerivedDummyProperty::DerivedDummyProperty(int value) : DummyProperty(), value(value) {}

std::unique_ptr<DummyProperty> DerivedDummyProperty::copy() const
{
	return std::make_unique<DerivedDummyProperty>(*this);
}

std::unique_ptr<DummyProperty> DerivedDummyProperty::move()
{
	return std::make_unique<DerivedDummyProperty>(std::move(*this));
}

size_t DerivedDummyProperty::hash() const
{
	return value;
}

std::ostream& DerivedDummyProperty::print(std::ostream& os) const
{
	return os << "DerivedDummyProperty(" << value << ")";
}

bool DerivedDummyProperty::is_equal_to(DummyProperty const& other) const
{
	return value == static_cast<DerivedDummyProperty const&>(other).value;
}

} // namespace pydapr
