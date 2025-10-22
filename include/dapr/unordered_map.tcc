#pragma once
#include "dapr/unordered_map.h"
#include <sstream>

namespace dapr {

template <
    typename Key,
    typename Value,
    template <typename...>
    typename ValueHolderT,
    template <typename...>
    typename KeyHolderT>
Value const& UnorderedMap<Key, Value, ValueHolderT, KeyHolderT>::get(Key const& key) const
{
	if (!m_values.contains(key)) {
		std::stringstream ss;
		ss << "UnorderedMap doesn't contain entry for key " << key << ".";
		throw std::out_of_range(ss.str());
	}
	if constexpr (std::is_base_of_v<Property<Value>, Value>) {
		return *m_values.at(key);
	} else {
		return m_values.at(key);
	}
}

template <
    typename Key,
    typename Value,
    template <typename...>
    typename ValueHolderT,
    template <typename...>
    typename KeyHolderT>
void UnorderedMap<Key, Value, ValueHolderT, KeyHolderT>::set(Key const& key, Value const& value)
{
	if (!m_values.contains(key)) {
		m_values.emplace(key, value);
	} else {
		m_values.at(key) = value;
	}
}

template <
    typename Key,
    typename Value,
    template <typename...>
    typename ValueHolderT,
    template <typename...>
    typename KeyHolderT>
void UnorderedMap<Key, Value, ValueHolderT, KeyHolderT>::set(Key const& key, Value&& value)
{
	if (!m_values.contains(key)) {
		m_values.emplace(key, std::move(value));
	} else {
		m_values.at(key) = std::move(value);
	}
}

template <
    typename Key,
    typename Value,
    template <typename...>
    typename ValueHolderT,
    template <typename...>
    typename KeyHolderT>
size_t UnorderedMap<Key, Value, ValueHolderT, KeyHolderT>::erase(Key const& key)
{
	return m_values.erase(key);
}

template <
    typename Key,
    typename Value,
    template <typename...>
    typename ValueHolderT,
    template <typename...>
    typename KeyHolderT>
bool UnorderedMap<Key, Value, ValueHolderT, KeyHolderT>::contains(Key const& key) const
{
	return m_values.contains(key);
}

template <
    typename Key,
    typename Value,
    template <typename...>
    typename ValueHolderT,
    template <typename...>
    typename KeyHolderT>
size_t UnorderedMap<Key, Value, ValueHolderT, KeyHolderT>::size() const
{
	return m_values.size();
}

template <
    typename Key,
    typename Value,
    template <typename...>
    typename ValueHolderT,
    template <typename...>
    typename KeyHolderT>
bool UnorderedMap<Key, Value, ValueHolderT, KeyHolderT>::empty() const
{
	return m_values.empty();
}

template <
    typename Key,
    typename Value,
    template <typename...>
    typename ValueHolderT,
    template <typename...>
    typename KeyHolderT>
void UnorderedMap<Key, Value, ValueHolderT, KeyHolderT>::merge(UnorderedMap& other)
{
	return m_values.merge(other.m_values);
}

template <
    typename Key,
    typename Value,
    template <typename...>
    typename ValueHolderT,
    template <typename...>
    typename KeyHolderT>
void UnorderedMap<Key, Value, ValueHolderT, KeyHolderT>::merge(UnorderedMap&& other)
{
	return m_values.merge(std::move(other.m_values));
}

template <
    typename Key,
    typename Value,
    template <typename...>
    typename ValueHolderT,
    template <typename...>
    typename KeyHolderT>
void UnorderedMap<Key, Value, ValueHolderT, KeyHolderT>::clear()
{
	return m_values.clear();
}

template <
    typename Key,
    typename Value,
    template <typename...>
    typename ValueHolderT,
    template <typename...>
    typename KeyHolderT>
typename UnorderedMap<Key, Value, ValueHolderT, KeyHolderT>::ConstIterator
UnorderedMap<Key, Value, ValueHolderT, KeyHolderT>::begin() const
{
	return ConstIterator(m_values.begin());
}

template <
    typename Key,
    typename Value,
    template <typename...>
    typename ValueHolderT,
    template <typename...>
    typename KeyHolderT>
typename UnorderedMap<Key, Value, ValueHolderT, KeyHolderT>::ConstIterator
UnorderedMap<Key, Value, ValueHolderT, KeyHolderT>::end() const
{
	return ConstIterator(m_values.end());
}

} // namespace dapr
