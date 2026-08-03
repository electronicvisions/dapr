#include "dapr/empty_hashable_property.h"

#include <vector>
#include <gtest/gtest.h>


namespace {

struct DummyEmptyHashableProperty
    : public dapr::Property<DummyEmptyHashableProperty>
    , public dapr::Hashable
{};


struct DerivedDummyEmptyHashableProperty
    : public dapr::
          EmptyHashableProperty<DerivedDummyEmptyHashableProperty, DummyEmptyHashableProperty>
{};

} // namespace

TEST(EmptyHashableProperty, General)
{
	DerivedDummyEmptyHashableProperty derived_dummy_1;
	DerivedDummyEmptyHashableProperty derived_dummy_2;

	EXPECT_TRUE(derived_dummy_1 == derived_dummy_1);
	EXPECT_FALSE(derived_dummy_1 != derived_dummy_1);

	EXPECT_TRUE(derived_dummy_1 == derived_dummy_2);
	EXPECT_FALSE(derived_dummy_1 != derived_dummy_2);

	std::stringstream ss;
	ss << derived_dummy_1;
	EXPECT_EQ(ss.str(), "DerivedDummyEmptyHashableProperty()");

	EXPECT_TRUE(derived_dummy_1.copy());
	EXPECT_TRUE(derived_dummy_1.move());

	EXPECT_NE(derived_dummy_1.hash(), 0);
}
