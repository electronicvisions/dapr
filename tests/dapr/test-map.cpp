#include "dapr/map.h"

#include "cereal/types/dapr/map.h"
#include "dapr/property.h"
#include <memory>
#include <cereal/archives/json.hpp>
#include <gtest/gtest.h>


namespace dapr_tests_map {

struct DummyProperty : public dapr::Property<DummyProperty>
{
	virtual ~DummyProperty() {}

	friend struct cereal::access;
	template <typename Archive>
	void serialize(Archive&)
	{
	}
};


struct DerivedDummyProperty : public DummyProperty
{
	int value;

	DerivedDummyProperty(int value = 0) : value(value) {}

	virtual std::unique_ptr<DummyProperty> copy() const override
	{
		return std::make_unique<DerivedDummyProperty>(*this);
	}

	virtual std::unique_ptr<DummyProperty> move() override
	{
		return std::make_unique<DerivedDummyProperty>(std::move(*this));
	}

	virtual bool is_equal_to(DummyProperty const& other) const override
	{
		return value == static_cast<DerivedDummyProperty const&>(other).value;
	}

	virtual std::ostream& print(std::ostream& os) const override
	{
		return os << "DerivedDummyProperty(" << value << ")";
	}

private:
	friend struct cereal::access;
	template <typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<DummyProperty>(this));
		ar(value);
	}
};

} // namespace dapr_tests_map

using namespace dapr_tests_map;


TEST(Map, PolymorphicValue)
{
	DerivedDummyProperty const dummy(5);

	dapr::Map<int, DummyProperty> map;

	EXPECT_TRUE(map.empty());
	EXPECT_EQ(map.size(), 0);
	EXPECT_FALSE(map.contains(0));

	map.set(0, dummy);

	EXPECT_FALSE(map.empty());
	EXPECT_EQ(map.size(), 1);
	EXPECT_TRUE(map.contains(0));
	EXPECT_EQ(map.get(0), dummy);

	auto dummy_to_move = dummy;
	map.set(1, std::move(dummy_to_move));

	EXPECT_EQ(map.size(), 2);
	EXPECT_TRUE(map.contains(1));
	EXPECT_EQ(map.get(1), dummy);

	auto dummy_copy = dummy;
	dummy_copy.value = 7;
	map.set(0, dummy_copy);

	EXPECT_EQ(map.get(0), dummy_copy);

	dummy_copy.value = 9;
	map.set(1, std::move(dummy_copy));

	EXPECT_EQ(dynamic_cast<DerivedDummyProperty const&>(map.get(1)).value, 9);

	std::map<int, DerivedDummyProperty> plain_map_copy;
	for (auto const& [key, value] : map) {
		plain_map_copy.emplace(key, dynamic_cast<DerivedDummyProperty const&>(value));
	}
	EXPECT_TRUE(plain_map_copy.contains(0));
	EXPECT_TRUE(plain_map_copy.contains(1));
	EXPECT_EQ(plain_map_copy.at(0).value, 7);
	EXPECT_EQ(plain_map_copy.at(1).value, 9);

	map.erase(1);
	EXPECT_FALSE(map.contains(1));
	EXPECT_EQ(map.size(), 1);

	auto map_copy = map;
	EXPECT_EQ(map, map_copy);

	map_copy.set(0, dummy);
	EXPECT_NE(map, map_copy);

	EXPECT_THROW(map.get(3), std::out_of_range);

	dapr::Map<int, DummyProperty> new_map;
	new_map.merge(map);
	EXPECT_TRUE(map.empty());

	map.merge(std::move(new_map));
	EXPECT_TRUE(new_map.empty());

	dapr::Map<int, DummyProperty> overlapping_map;
	overlapping_map.set(0, dummy);
	map.merge(overlapping_map);
	EXPECT_EQ(overlapping_map.size(), 1);
	dummy_copy.value = 7;
	EXPECT_EQ(map.get(0), dummy_copy);
	EXPECT_EQ(overlapping_map.get(0), dummy);

	map.clear();
	EXPECT_TRUE(map.empty());
}

TEST(Map, NotPolymorphic)
{
	dapr::Map<int, int> map;

	EXPECT_TRUE(map.empty());
	EXPECT_EQ(map.size(), 0);
	EXPECT_FALSE(map.contains(1));

	map.set(1, 0);

	EXPECT_FALSE(map.empty());
	EXPECT_EQ(map.size(), 1);
	EXPECT_TRUE(map.contains(1));
	EXPECT_EQ(map.get(1), 0);

	map.set(7, 1);

	EXPECT_EQ(map.get(7), 1);

	map.erase(7);
	EXPECT_FALSE(map.contains(7));
	EXPECT_EQ(map.size(), 1);

	auto map_copy = map;
	EXPECT_EQ(map, map_copy);

	map_copy.set(1, 1);
	EXPECT_NE(map, map_copy);

	EXPECT_THROW(map.get(7), std::out_of_range);

	dapr::Map<int, int> new_map;
	new_map.merge(map);
	EXPECT_TRUE(map.empty());

	map.merge(std::move(new_map));
	EXPECT_TRUE(new_map.empty());

	dapr::Map<int, int> overlapping_map;
	overlapping_map.set(1, 1);
	map.merge(overlapping_map);
	EXPECT_EQ(overlapping_map.size(), 1);
	EXPECT_EQ(map.get(1), 0);
	EXPECT_EQ(overlapping_map.get(1), 1);

	map.clear();
	EXPECT_TRUE(map.empty());
}

CEREAL_REGISTER_TYPE(dapr_tests_map::DummyProperty)
CEREAL_REGISTER_TYPE(dapr_tests_map::DerivedDummyProperty)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    dapr_tests_map::DummyProperty, dapr_tests_map::DerivedDummyProperty)

TEST(Map, PolymorphicValueCerealization)
{
	DerivedDummyProperty const dummy(5);

	dapr::Map<int, DummyProperty> obj;
	obj.set(0, dummy);

	dapr::Map<int, DummyProperty> obj2;

	std::ostringstream ostream;
	{
		cereal::JSONOutputArchive oa(ostream);
		oa(obj);
	}

	std::istringstream istream(ostream.str());
	{
		cereal::JSONInputArchive ia(istream);
		ia(obj2);
	}

	ASSERT_EQ(obj2, obj);
}

TEST(Map, ValueCerealization)
{
	dapr::Map<int, int> obj;
	obj.set(0, 1);

	dapr::Map<int, int> obj2;

	std::ostringstream ostream;
	{
		cereal::JSONOutputArchive oa(ostream);
		oa(obj);
	}

	std::istringstream istream(ostream.str());
	{
		cereal::JSONInputArchive ia(istream);
		ia(obj2);
	}

	ASSERT_EQ(obj2, obj);
}
