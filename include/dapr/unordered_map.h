#pragma once
#include "dapr/detail/unordered_map_transform.h"
#include "dapr/genpybind.h"
#include "dapr/hashable.h"
#include "dapr/property.h"
#include "dapr/property_holder.h"
#include <functional>
#include <map>
#include <optional>
#include <unordered_map>
#include <boost/iterator/transform_iterator.hpp>

namespace cereal {
struct access;
} // namespace cereal

namespace dapr {

/**
 * UnorderedMap storing potentially polymorphic keys and/or values.
 * The underlying map implementation is an unordered map.
 * @tparam KeyT Key type, which if polymorphic is required to be derived from a Property and
 * Hashable
 * @tparam ValueT Value type, which if polymorphic is required to be derived from a Property
 */
template <typename KeyT, typename ValueT>
struct GENPYBIND(visible) UnorderedMap
{
	typedef KeyT Key;
	typedef ValueT Value;
	typedef std::unordered_map<
	    std::conditional_t<
	        std::is_base_of_v<Property<Key>, Key> && std::is_base_of_v<Hashable, Key>,
	        PropertyHolder<Key>,
	        Key>,
	    std::conditional_t<std::is_base_of_v<Property<Value>, Value>, PropertyHolder<Value>, Value>>
	    Backend;

	UnorderedMap() = default;

	/**
	 * Get value of element present in map.
	 * @param key Key for which to get value
	 * @return Value to get
	 * @throws std::out_of_range On no element for key present in map
	 */
	Value const& get(Key const& key) const GENPYBIND(hidden);

	GENPYBIND_MANUAL({
		typedef typename decltype(parent)::type self_type;
		parent.def(
		    "get",
		    [](GENPYBIND_PARENT_TYPE const& self, typename self_type::Key const& key) ->
		    typename self_type::Value const& { return self.get(key); },
		    parent->py::return_value_policy::reference_internal);
	})

	/**
	 * Add or set value of element in map.
	 * @param key Key for which to get value
	 * @param value Value to get
	 */
	void set(Key const& key, Value const& value);

	/**
	 * Add or set value of element in map.
	 * @param key Key for which to get value
	 * @param value Value to get
	 */
	void set(Key const& key, Value&& value) GENPYBIND(hidden);

	/**
	 * Erase element for given key in map.
	 * @param key Key to erase element for
	 */
	size_t erase(Key const& key);

	/**
	 * Get whether element for given key is present in map.
	 * @param key Key to get property for
	 */
	bool contains(Key const& key) const;

	/**
	 * Get number of elements present in map.
	 */
	size_t size() const;

	/**
	 * Get whether map is empty and contains no elements.
	 */
	bool empty() const;

	void merge(UnorderedMap& other);
	void merge(UnorderedMap&& other) GENPYBIND(hidden);

	typedef boost::transform_iterator<
	    typename detail::UnorderedMapTransform<Key, Value>,
	    typename Backend::const_iterator>
	    ConstIterator;

	/**
	 * Get begin iterator to elements in map.
	 */
	ConstIterator begin() const GENPYBIND(hidden);

	/**
	 * Get end iterator to elements in map.
	 */
	ConstIterator end() const GENPYBIND(hidden);

	bool operator==(UnorderedMap const& other) const = default;
	bool operator!=(UnorderedMap const& other) const = default;

private:
	Backend m_values;

	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive&, std::uint32_t);
};

} // namespace dapr

#include "dapr/unordered_map.tcc"
