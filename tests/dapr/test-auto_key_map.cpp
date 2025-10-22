#include "dapr/auto_key_map.h"

#include "cereal/types/dapr/auto_key_map.h"
#include "dapr/property.h"
#include <memory>
#include <cereal/archives/json.hpp>
#include <gtest/gtest.h>


namespace dapr_tests_auto_key_map {

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

} // namespace dapr_tests_auto_key_map

using namespace dapr_tests_auto_key_map;


TEST(AutoKeyMap, PolymorphicValue)
{
	DerivedDummyProperty const dummy(5);

	dapr::AutoKeyMap<int, DummyProperty> map;

	EXPECT_TRUE(map.empty());
	EXPECT_EQ(map.size(), 0);
	EXPECT_FALSE(map.contains(0));
	EXPECT_THROW(map.set(0, dummy), std::out_of_range);

	auto key_0 = map.insert(dummy);
	EXPECT_EQ(key_0, 0);

	EXPECT_FALSE(map.empty());
	EXPECT_EQ(map.size(), 1);
	EXPECT_TRUE(map.contains(key_0));
	EXPECT_EQ(map.get(key_0), dummy);

	auto dummy_to_move = dummy;
	auto key_1 = map.insert(std::move(dummy_to_move));
	EXPECT_EQ(key_1, 1);

	EXPECT_EQ(map.size(), 2);
	EXPECT_TRUE(map.contains(key_1));
	EXPECT_EQ(map.get(key_1), dummy);

	auto dummy_copy = dummy;
	dummy_copy.value = 7;
	map.set(key_0, dummy_copy);

	EXPECT_EQ(map.get(key_0), dummy_copy);

	dummy_copy.value = 9;
	map.set(key_1, std::move(dummy_copy));

	EXPECT_EQ(dynamic_cast<DerivedDummyProperty const&>(map.get(key_1)).value, 9);

	std::map<int, DerivedDummyProperty> plain_map_copy;
	for (auto const& [key, value] : map) {
		plain_map_copy.emplace(key, dynamic_cast<DerivedDummyProperty const&>(value));
	}
	EXPECT_TRUE(plain_map_copy.contains(0));
	EXPECT_TRUE(plain_map_copy.contains(1));
	EXPECT_EQ(plain_map_copy.at(0).value, 7);
	EXPECT_EQ(plain_map_copy.at(1).value, 9);

	map.erase(key_1);
	EXPECT_FALSE(map.contains(key_1));
	EXPECT_EQ(map.size(), 1);

	auto key_2 = map.insert(dummy);
	EXPECT_EQ(key_2, 2);

	auto map_copy = map;
	EXPECT_EQ(map, map_copy);

	map_copy.set(key_0, dummy);
	EXPECT_NE(map, map_copy);

	EXPECT_THROW(map.get(3), std::out_of_range);
}

TEST(AutoKeyMap, Value)
{
	dapr::AutoKeyMap<int, int> map;

	EXPECT_TRUE(map.empty());
	EXPECT_EQ(map.size(), 0);
	EXPECT_FALSE(map.contains(0));
	EXPECT_THROW(map.set(0, 0), std::out_of_range);

	auto key_0 = map.insert(0);
	EXPECT_EQ(key_0, 0);

	EXPECT_FALSE(map.empty());
	EXPECT_EQ(map.size(), 1);
	EXPECT_TRUE(map.contains(key_0));
	EXPECT_EQ(map.get(key_0), 0);

	auto key_1 = map.insert(1);

	EXPECT_EQ(map.get(key_1), 1);

	map.erase(key_1);
	EXPECT_FALSE(map.contains(key_1));
	EXPECT_EQ(map.size(), 1);

	auto key_2 = map.insert(2);
	EXPECT_EQ(key_2, 2);

	auto map_copy = map;
	EXPECT_EQ(map, map_copy);

	map_copy.set(key_0, 1);
	EXPECT_NE(map, map_copy);

	EXPECT_THROW(map.get(7), std::out_of_range);
}

CEREAL_REGISTER_TYPE(dapr_tests_auto_key_map::DummyProperty)
CEREAL_REGISTER_TYPE(dapr_tests_auto_key_map::DerivedDummyProperty)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    dapr_tests_auto_key_map::DummyProperty, dapr_tests_auto_key_map::DerivedDummyProperty)

TEST(AutoKeyMap, PolymorphicValueCerealization)
{
	DerivedDummyProperty const dummy(5);

	dapr::AutoKeyMap<int, DummyProperty> obj;
	obj.insert(dummy);

	dapr::AutoKeyMap<int, DummyProperty> obj2;

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

TEST(AutoKeyMap, ValueCerealization)
{
	dapr::AutoKeyMap<int, int> obj;
	obj.insert(1);

	dapr::AutoKeyMap<int, int> obj2;

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
