#pragma once
#include "dapr/property.h"
#include <iosfwd>
#include <memory>
#include <type_traits>

namespace cereal {
struct access;
} // namespace cereal


namespace dapr {

/**
 * Holder type for polymorphic property.
 * It is copyable, value-equality comparable, printable and provides safe access to the stored
 * object.
 * @tparam T Type of object to store
 * @tparam Backend Backend storage type
 */
template <typename T, template <typename...> typename Backend = std::unique_ptr>
struct PropertyHolder
{
	PropertyHolder() = default;

	/**
	 * Construct property holder.
	 * @param value Value to store
	 */
	PropertyHolder(T const& value);

	/**
	 * Construct property holder.
	 * @param value Value to store
	 */
	PropertyHolder(T&& value);

	/**
	 * Copy-construct property holder.
	 * @param other Other property holder
	 */
	PropertyHolder(PropertyHolder const& other);

	/**
	 * Move-construct property holder.
	 * @param other Other property holder
	 */
	PropertyHolder(PropertyHolder&& other) = default;

	/**
	 * Copy-assign property holder.
	 * @param other Other property holder
	 */
	PropertyHolder& operator=(PropertyHolder const& other);

	/**
	 * Move-assign property holder.
	 * @param other Other property holder
	 */
	PropertyHolder& operator=(PropertyHolder&& other) = default;

	/**
	 * Copy-assign property holder.
	 * @param other Other property holder
	 */
	PropertyHolder& operator=(T const& other);

	/**
	 * Move-assign property holder.
	 * @param other Other property holder
	 */
	PropertyHolder& operator=(T&& other);

	/**
	 * Get whether object is present.
	 */
	explicit operator bool() const;

	/**
	 * Get reference to stored object.
	 * @throws std::runtime_error On no object being present
	 */
	T& operator*() const;

	/**
	 * Get pointer to stored object.
	 * @throws std::runtime_error On no object being present
	 */
	T* operator->() const;

	/**
	 * Value-compare stored object if present.
	 * @param other Other property holder
	 */
	bool operator==(PropertyHolder const& other) const;

	/**
	 * Value-compare stored object if present.
	 * @param other Other property holder
	 */
	bool operator!=(PropertyHolder const& other) const;

	/**
	 * If the stored type is Hashable, this is forwarded, otherwise the implementation of the
	 * Backend is used.
	 */
	size_t hash() const;

private:
	Backend<T> m_backend;

	/**
	 * For the property holder to be copyable, movable, comparable and printable, the stored type is
	 * required to be a property.
	 */
	static_assert(std::is_base_of_v<Property<T>, T>);

	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t);
};

template <typename T, template <typename...> typename Backend>
std::ostream& operator<<(std::ostream& os, PropertyHolder<T, Backend> const& value);

} // namespace dapr

namespace std {

template <typename T>
struct hash;

template <typename T, template <typename...> typename Backend>
struct hash<dapr::PropertyHolder<T, Backend>>
{
	size_t operator()(dapr::PropertyHolder<T, Backend> const& value) const
	{
		return value.hash();
	}
};

} // namespace std

#include "dapr/property_holder.tcc"
