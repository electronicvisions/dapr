#pragma once
#include "dapr/map.h"
#include <cassert>
#include <sstream>

namespace dapr {

template <typename Key, typename Value, template <typename...> typename HolderT>
Value const& Map<Key, Value, HolderT>::get(Key const& key) const
{
	if (!m_values.contains(key)) {
		std::stringstream ss;
		ss << "Map doesn't contain entry for key " << key << ".";
		throw std::out_of_range(ss.str());
	}
	if constexpr (std::is_base_of_v<Property<Value>, Value>) {
		return *m_values.at(key);
	} else {
		return m_values.at(key);
	}
}

template <typename Key, typename Value, template <typename...> typename HolderT>
void Map<Key, Value, HolderT>::set(Key const& key, Value const& value)
{
	if (!m_values.contains(key)) {
		m_values.emplace(key, value);
	} else {
		m_values.at(key) = value;
	}
}

template <typename Key, typename Value, template <typename...> typename HolderT>
void Map<Key, Value, HolderT>::set(Key const& key, Value&& value)
{
	if (!m_values.contains(key)) {
		m_values.emplace(key, std::move(value));
	} else {
		m_values.at(key) = std::move(value);
	}
}

template <typename Key, typename Value, template <typename...> typename HolderT>
size_t Map<Key, Value, HolderT>::erase(Key const& key)
{
	return m_values.erase(key);
}

template <typename Key, typename Value, template <typename...> typename HolderT>
bool Map<Key, Value, HolderT>::contains(Key const& key) const
{
	return m_values.contains(key);
}

template <typename Key, typename Value, template <typename...> typename HolderT>
size_t Map<Key, Value, HolderT>::size() const
{
	return m_values.size();
}

template <typename Key, typename Value, template <typename...> typename HolderT>
bool Map<Key, Value, HolderT>::empty() const
{
	return m_values.empty();
}

template <typename Key, typename Value, template <typename...> typename HolderT>
void Map<Key, Value, HolderT>::merge(Map& other)
{
	return m_values.merge(other.m_values);
}

template <typename Key, typename Value, template <typename...> typename HolderT>
void Map<Key, Value, HolderT>::merge(Map&& other)
{
	return m_values.merge(std::move(other.m_values));
}

template <typename Key, typename Value, template <typename...> typename HolderT>
void Map<Key, Value, HolderT>::clear()
{
	return m_values.clear();
}

template <typename Key, typename Value, template <typename...> typename HolderT>
typename Map<Key, Value, HolderT>::ConstIterator Map<Key, Value, HolderT>::begin() const
{
	return ConstIterator(m_values.begin());
}

template <typename Key, typename Value, template <typename...> typename HolderT>
typename Map<Key, Value, HolderT>::ConstIterator Map<Key, Value, HolderT>::end() const
{
	return ConstIterator(m_values.end());
}

} // namespace dapr
