#include "dapr/empty_property.h"

#include <vector>
#include <gtest/gtest.h>


namespace {

struct DummyEmptyProperty : public dapr::Property<DummyEmptyProperty>
{};


struct DerivedDummyEmptyProperty
    : public dapr::EmptyProperty<DerivedDummyEmptyProperty, DummyEmptyProperty>
{};

} // namespace

TEST(EmptyProperty, General)
{
	DerivedDummyEmptyProperty derived_dummy_1;
	DerivedDummyEmptyProperty derived_dummy_2;

	EXPECT_TRUE(derived_dummy_1 == derived_dummy_1);
	EXPECT_FALSE(derived_dummy_1 != derived_dummy_1);

	EXPECT_TRUE(derived_dummy_1 == derived_dummy_2);
	EXPECT_FALSE(derived_dummy_1 != derived_dummy_2);

	std::stringstream ss;
	ss << derived_dummy_1;
	EXPECT_EQ(ss.str(), "DerivedDummyEmptyProperty()");

	EXPECT_TRUE(derived_dummy_1.copy());
	EXPECT_TRUE(derived_dummy_1.move());
}
