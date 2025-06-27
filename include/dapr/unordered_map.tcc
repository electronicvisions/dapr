#pragma once
#include "dapr/unordered_map.h"
#include <sstream>

namespace dapr {

template <typename Key, typename Value>
Value const& UnorderedMap<Key, Value>::get(Key const& key) const
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

template <typename Key, typename Value>
void UnorderedMap<Key, Value>::set(Key const& key, Value const& value)
{
	if (!m_values.contains(key)) {
		m_values.emplace(key, value);
	} else {
		m_values.at(key) = value;
	}
}

template <typename Key, typename Value>
void UnorderedMap<Key, Value>::set(Key const& key, Value&& value)
{
	if (!m_values.contains(key)) {
		m_values.emplace(key, std::move(value));
	} else {
		m_values.at(key) = std::move(value);
	}
}

template <typename Key, typename Value>
size_t UnorderedMap<Key, Value>::erase(Key const& key)
{
	return m_values.erase(key);
}

template <typename Key, typename Value>
bool UnorderedMap<Key, Value>::contains(Key const& key) const
{
	return m_values.contains(key);
}

template <typename Key, typename Value>
size_t UnorderedMap<Key, Value>::size() const
{
	return m_values.size();
}

template <typename Key, typename Value>
bool UnorderedMap<Key, Value>::empty() const
{
	return m_values.empty();
}

template <typename Key, typename Value>
typename UnorderedMap<Key, Value>::ConstIterator UnorderedMap<Key, Value>::begin() const
{
	return ConstIterator(m_values.begin());
}

template <typename Key, typename Value>
typename UnorderedMap<Key, Value>::ConstIterator UnorderedMap<Key, Value>::end() const
{
	return ConstIterator(m_values.end());
}

} // namespace dapr
