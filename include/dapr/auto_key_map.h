#pragma once
#include "dapr/genpybind.h"
#include "dapr/map.h"
#include <limits>

namespace cereal {
struct access;
} // namespace cereal


namespace dapr {

/**
 * Map storing potentially polymorphic values and automatically generating keys upon insertion of a
 * value.
 * @tparam KeyT Key type required to be an arithmetic type supporting operator{++,==} and
 * construction from `1`
 * @tparam ValueT Value type
 */
template <typename KeyT, typename ValueT, typename BackendT = Map<KeyT, ValueT>>
struct GENPYBIND(visible) AutoKeyMap
{
	typedef KeyT Key;
	typedef ValueT Value;
	typedef BackendT Backend;

	AutoKeyMap() = default;

	/**
	 * Insert new value.
	 * @param value Value to insert
	 * @return Automatically generated key
	 * @throws std::out_of_range If no key could be automatically generated due to the next key
	 * being out of range of the numerical limits of the key.
	 */
	Key insert(Value const& value);

	/**
	 * Insert new value.
	 * @param value Value to insert
	 * @return Automatically generated key
	 * @throws std::out_of_range If no key could be automatically generated due to the next key
	 * being out of range of the numerical limits of the key.
	 */
	Key insert(Value&& value) GENPYBIND(hidden);

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
	 * Set value of element in map.
	 * @param key Key for which to get value
	 * @param value Value to get
	 * @throws std::out_of_range If no value to given key is stored
	 */
	void set(Key const& key, Value const& value);

	/**
	 * Setet value of element in map.
	 * @param key Key for which to get value
	 * @param value Value to get
	 * @throws std::out_of_range If no value to given key is stored
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

	/**
	 * Remove all elements in the map.
	 */
	void clear();

	typedef typename Backend::ConstIterator ConstIterator;

	/**
	 * Get begin iterator to elements in map.
	 */
	ConstIterator begin() const GENPYBIND(hidden);

	/**
	 * Get end iterator to elements in map.
	 */
	ConstIterator end() const GENPYBIND(hidden);

	bool operator==(AutoKeyMap const& other) const = default;
	bool operator!=(AutoKeyMap const& other) const = default;

private:
	Backend m_values;
	Key m_next_key{};

	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive&, std::uint32_t);
};

} // namespace dapr

#include "dapr/auto_key_map.tcc"
