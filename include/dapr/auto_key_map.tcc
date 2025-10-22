#pragma once
#include "dapr/auto_key_map.h"
#include <cassert>
#include <limits>
#include <sstream>

namespace dapr {

template <typename Key, typename Value, typename Backend>
Key AutoKeyMap<Key, Value, Backend>::insert(Value const& value)
{
	if (m_next_key == std::numeric_limits<Key>::max()) {
		throw std::out_of_range("AutoKeyMap can't generate next key.");
	}
	auto const key = m_next_key;
	m_next_key += Key(1);
	m_values.set(key, value);
	return key;
}

template <typename Key, typename Value, typename Backend>
Key AutoKeyMap<Key, Value, Backend>::insert(Value&& value)
{
	if (m_next_key == std::numeric_limits<Key>::max()) {
		throw std::out_of_range("AutoKeyMap can't generate next key.");
	}
	auto const key = m_next_key;
	m_next_key += Key(1);
	m_values.set(key, std::move(value));
	return key;
}

template <typename Key, typename Value, typename Backend>
Value const& AutoKeyMap<Key, Value, Backend>::get(Key const& key) const
{
	return m_values.get(key);
}

template <typename Key, typename Value, typename Backend>
void AutoKeyMap<Key, Value, Backend>::set(Key const& key, Value const& value)
{
	if (!m_values.contains(key)) {
		std::stringstream ss;
		ss << "AutoKeyMap doesn't contain entry for key " << key << ".";
		throw std::out_of_range(ss.str());
	}
	return m_values.set(key, value);
}

template <typename Key, typename Value, typename Backend>
void AutoKeyMap<Key, Value, Backend>::set(Key const& key, Value&& value)
{
	if (!m_values.contains(key)) {
		std::stringstream ss;
		ss << "AutoKeyMap doesn't contain entry for key " << key << ".";
		throw std::out_of_range(ss.str());
	}
	m_values.set(key, std::move(value));
}

template <typename Key, typename Value, typename Backend>
size_t AutoKeyMap<Key, Value, Backend>::erase(Key const& key)
{
	return m_values.erase(key);
}

template <typename Key, typename Value, typename Backend>
bool AutoKeyMap<Key, Value, Backend>::contains(Key const& key) const
{
	return m_values.contains(key);
}

template <typename Key, typename Value, typename Backend>
size_t AutoKeyMap<Key, Value, Backend>::size() const
{
	return m_values.size();
}

template <typename Key, typename Value, typename Backend>
bool AutoKeyMap<Key, Value, Backend>::empty() const
{
	return m_values.empty();
}

template <typename Key, typename Value, typename Backend>
void AutoKeyMap<Key, Value, Backend>::clear()
{
	m_values.clear();
	m_next_key = Key();
}

template <typename Key, typename Value, typename Backend>
typename AutoKeyMap<Key, Value, Backend>::Backend::ConstIterator
AutoKeyMap<Key, Value, Backend>::begin() const
{
	return m_values.begin();
}

template <typename Key, typename Value, typename Backend>
typename AutoKeyMap<Key, Value, Backend>::Backend::ConstIterator
AutoKeyMap<Key, Value, Backend>::end() const
{
	return m_values.end();
}

} // namespace dapr
