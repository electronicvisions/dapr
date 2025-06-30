#include "dapr/hashable.h"

#include "dapr/empty_property.h"
#include "dapr/empty_property_impl.tcc"
#include "dapr/property.h"
#include "dapr/property_holder.h"
#include <gtest/gtest.h>


namespace {

struct DummyHashable : public dapr::Hashable
{
	size_t value;

	DummyHashable(size_t value) : value(value) {}

protected:
	virtual size_t hash() const override
	{
		return value;
	}
};

} // namespace


TEST(Hashable, General)
{
	DummyHashable dummy_5(5);

	EXPECT_EQ(std::hash<dapr::Hashable>{}(dummy_5), 5);
}
