#pragma once
#include "dapr/property.h"
#include "hate/visibility.h"
#include "pydapr/genpybind.h"

namespace pydapr GENPYBIND_TAG_PYDAPR {

struct SYMBOL_VISIBLE GENPYBIND(inline_base("*")) DummyProperty
    : public dapr::Property<DummyProperty>
{
	virtual ~DummyProperty();
};


struct SYMBOL_VISIBLE GENPYBIND(visible) DerivedDummyProperty : public DummyProperty
{
	int value;

	DerivedDummyProperty(int value);

	virtual std::unique_ptr<DummyProperty> copy() const override;
	virtual std::unique_ptr<DummyProperty> move() override;

protected:
	virtual bool is_equal_to(DummyProperty const& other) const override;
	virtual std::ostream& print(std::ostream& os) const override;
};

} // namespace pydapr
