#pragma once

#include <vector>
#include <string>

#include "Database/Armors.hpp"
#include "Database/Weapons.hpp"
/*!
 * @struct ObjectPicker
 * @details Provides a pickable list broken up into groups of 100
 * @tparam T Object type to list
 */
template <typename T>
struct ObjectPicker {
  ObjectPicker(std::string_view objectType, std::vector<T>& list, const int initialSelection)
  : m_objectType(objectType), m_list(&list), m_selection(initialSelection) {}

  std::tuple<bool, bool> draw();

  int selection() const { return m_selection; }

  int getId(const T& value) { return value.id; }

  const std::string& getName(const T& value) { return value.name; }

private:
  std::string m_objectType;
  std::vector<T>* m_list;
  bool m_confirmed{false};
  int m_selection{1};
  bool m_open = true;
  std::string m_filter;
};

struct ObjectPickerList {
  ObjectPickerList(int groupCount);
};

#include "ObjectPicker.inl"