#include "dapr/unordered_map.h"

#include "cereal/types/dapr/unordered_map.h"
#include "dapr/hashable.h"
#include "dapr/property.h"
#include <memory>
#include <cereal/archives/json.hpp>
#include <gtest/gtest.h>

namespace dapr_tests_unordered_map {

struct DummyProperty
    : public dapr::Property<DummyProperty>
    , dapr::Hashable
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
	int value{};

	DerivedDummyProperty() = default;
	DerivedDummyProperty(int value) : value(value) {}

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

	virtual size_t hash() const override
	{
		return value;
	}

	friend struct cereal::access;
	template <typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<DummyProperty>(this));
		ar(value);
	}
};

} // namespace dapr_tests_unordered_map

using namespace dapr_tests_unordered_map;

namespace std {

template <>
struct hash<DerivedDummyProperty>
{
	size_t operator()(DerivedDummyProperty const& value) const
	{
		return value.hash();
	}
};

} // namespace std


TEST(UnorderedMap, PolymorphicValue)
{
	DerivedDummyProperty const dummy(5);

	dapr::UnorderedMap<int, DummyProperty> map;

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

	dapr::UnorderedMap<int, DummyProperty> new_map;
	new_map.merge(map);
	EXPECT_TRUE(map.empty());

	map.merge(std::move(new_map));
	EXPECT_TRUE(new_map.empty());

	dapr::UnorderedMap<int, DummyProperty> overlapping_map;
	overlapping_map.set(0, dummy);
	map.merge(overlapping_map);
	EXPECT_EQ(overlapping_map.size(), 1);
	dummy_copy.value = 7;
	EXPECT_EQ(map.get(0), dummy_copy);
	EXPECT_EQ(overlapping_map.get(0), dummy);
}

TEST(UnorderedMap, PolymorphicKey)
{
	DerivedDummyProperty const dummy(5);

	dapr::UnorderedMap<DummyProperty, int> map;

	EXPECT_TRUE(map.empty());
	EXPECT_EQ(map.size(), 0);
	EXPECT_FALSE(map.contains(dummy));

	map.set(dummy, 0);

	EXPECT_FALSE(map.empty());
	EXPECT_EQ(map.size(), 1);
	EXPECT_TRUE(map.contains(dummy));
	EXPECT_EQ(map.get(dummy), 0);

	auto dummy_copy = dummy;
	dummy_copy.value = 7;
	map.set(dummy_copy, 1);

	EXPECT_EQ(map.get(dummy_copy), 1);

	map.erase(dummy_copy);
	EXPECT_FALSE(map.contains(dummy_copy));
	EXPECT_EQ(map.size(), 1);

	auto map_copy = map;
	EXPECT_EQ(map, map_copy);

	map_copy.set(dummy, 1);
	EXPECT_NE(map, map_copy);

	EXPECT_THROW(map.get(dummy_copy), std::out_of_range);

	dapr::UnorderedMap<DummyProperty, int> new_map;
	new_map.merge(map);
	EXPECT_TRUE(map.empty());

	map.merge(std::move(new_map));
	EXPECT_TRUE(new_map.empty());

	dapr::UnorderedMap<DummyProperty, int> overlapping_map;
	overlapping_map.set(dummy, 1);
	map.merge(overlapping_map);
	EXPECT_EQ(overlapping_map.size(), 1);
	EXPECT_EQ(map.get(dummy), 0);
	EXPECT_EQ(overlapping_map.get(dummy), 1);
}

TEST(UnorderedMap, PolymorphicKeyValue)
{
	DerivedDummyProperty const dummy_0(0);
	DerivedDummyProperty const dummy_1(1);
	DerivedDummyProperty const dummy(5);

	dapr::UnorderedMap<DummyProperty, DummyProperty> map;

	EXPECT_TRUE(map.empty());
	EXPECT_EQ(map.size(), 0);
	EXPECT_FALSE(map.contains(dummy_0));

	map.set(dummy_0, dummy);

	EXPECT_FALSE(map.empty());
	EXPECT_EQ(map.size(), 1);
	EXPECT_TRUE(map.contains(dummy_0));
	EXPECT_EQ(map.get(dummy_0), dummy);

	auto dummy_to_move = dummy;
	map.set(dummy_1, std::move(dummy_to_move));

	EXPECT_EQ(map.size(), 2);
	EXPECT_TRUE(map.contains(dummy_1));
	EXPECT_EQ(map.get(dummy_1), dummy);

	auto dummy_copy = dummy;
	dummy_copy.value = 7;
	map.set(dummy_0, dummy_copy);

	EXPECT_EQ(map.get(dummy_0), dummy_copy);

	dummy_copy.value = 9;
	map.set(dummy_1, std::move(dummy_copy));

	EXPECT_EQ(dynamic_cast<DerivedDummyProperty const&>(map.get(dummy_1)).value, 9);

	std::unordered_map<DerivedDummyProperty, DerivedDummyProperty> plain_map_copy;
	for (auto const& [key, value] : map) {
		plain_map_copy.emplace(
		    dynamic_cast<DerivedDummyProperty const&>(key),
		    dynamic_cast<DerivedDummyProperty const&>(value));
	}
	EXPECT_TRUE(plain_map_copy.contains(0));
	EXPECT_TRUE(plain_map_copy.contains(1));
	EXPECT_EQ(plain_map_copy.at(0).value, 7);
	EXPECT_EQ(plain_map_copy.at(1).value, 9);

	map.erase(dummy_1);
	EXPECT_FALSE(map.contains(dummy_1));
	EXPECT_EQ(map.size(), 1);

	auto map_copy = map;
	EXPECT_EQ(map, map_copy);

	map_copy.set(dummy_0, dummy);
	EXPECT_NE(map, map_copy);

	EXPECT_THROW(map.get(DerivedDummyProperty(3)), std::out_of_range);

	dapr::UnorderedMap<DummyProperty, DummyProperty> new_map;
	new_map.merge(map);
	EXPECT_TRUE(map.empty());

	map.merge(std::move(new_map));
	EXPECT_TRUE(new_map.empty());

	dapr::UnorderedMap<DummyProperty, DummyProperty> overlapping_map;
	overlapping_map.set(dummy_0, dummy);
	map.merge(overlapping_map);
	EXPECT_EQ(overlapping_map.size(), 1);
	dummy_copy.value = 7;
	EXPECT_EQ(map.get(dummy_0), dummy_copy);
	EXPECT_EQ(overlapping_map.get(dummy_0), dummy);
}

TEST(UnorderedMap, NotPolymorphic)
{
	dapr::UnorderedMap<int, int> map;

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

	dapr::UnorderedMap<int, int> new_map;
	new_map.merge(map);
	EXPECT_TRUE(map.empty());

	map.merge(std::move(new_map));
	EXPECT_TRUE(new_map.empty());

	dapr::UnorderedMap<int, int> overlapping_map;
	overlapping_map.set(1, 1);
	map.merge(overlapping_map);
	EXPECT_EQ(overlapping_map.size(), 1);
	EXPECT_EQ(map.get(1), 0);
	EXPECT_EQ(overlapping_map.get(1), 1);
}

CEREAL_REGISTER_TYPE(dapr_tests_unordered_map::DummyProperty)
CEREAL_REGISTER_TYPE(dapr_tests_unordered_map::DerivedDummyProperty)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    dapr_tests_unordered_map::DummyProperty, dapr_tests_unordered_map::DerivedDummyProperty)

TEST(UnorderedMap, PolymorphicValueCerealization)
{
	DerivedDummyProperty const dummy(5);

	dapr::UnorderedMap<int, DummyProperty> obj;
	obj.set(0, dummy);

	dapr::UnorderedMap<int, DummyProperty> obj2;

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

TEST(UnorderedMap, PolymorphicKeyCerealization)
{
	DerivedDummyProperty const dummy(5);

	dapr::UnorderedMap<DummyProperty, int> obj;
	obj.set(dummy, 1);

	dapr::UnorderedMap<DummyProperty, int> obj2;

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

TEST(UnorderedMap, PolymorphicKeyValueCerealization)
{
	DerivedDummyProperty const dummy_5(5);
	DerivedDummyProperty const dummy_7(7);

	dapr::UnorderedMap<DummyProperty, DummyProperty> obj;
	obj.set(dummy_5, dummy_7);

	dapr::UnorderedMap<DummyProperty, DummyProperty> obj2;

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

TEST(UnorderedMap, NotPolymorphicCerealization)
{
	dapr::UnorderedMap<int, int> obj;
	obj.set(0, 1);

	dapr::UnorderedMap<int, int> obj2;

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
