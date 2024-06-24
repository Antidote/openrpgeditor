#pragma once

#include "Database/Globals.hpp"
#include "Database/Damage.hpp"
#include "Database/Effect.hpp"

#include <nlohmann/json.hpp>
#include <string>

class Item {
public:
  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Item, id, animationId, consumable, damage, description, effects, hitType,
                                              iconIndex, itypeId, name, note, occasion, price, scope, speed,
                                              successRate, tpGain);
  int id;
  int animationId;
  bool consumable;
  Damage damage;
  std::string description;
  std::vector<Effect> effects;
  ItemType hitType;
  int iconIndex;
  int itypeId;
  std::string name;
  std::string note;
  Occasion occasion;
  int price;
  int repeats;
  Scope scope;
  int speed;
  int successRate;
  int tpGain;

  /*!
   * @name m_isValid
   * @details
   * Indicates that this is an actual valid entry and not a dummy
   * When making a new entry make sure to set this to true or it won't be
   * serialized.
   */
  bool m_isValid{false};
};

class Items {
public:
  static Items load(std::string_view filename);
  void serialize(std::string_view filename);

  [[nodiscard]] Item* item(int id) {
    for (auto& item : m_items) {
      if (item.id == id && item.m_isValid) {
        return &item;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const Item* item(int id) const {
    for (const auto& item : m_items) {
      if (item.id == id && item.m_isValid) {
        return &item;
      }
    }

    return nullptr;
  }
  std::vector<Item>& itemList() { return m_items; }
  const std::vector<Item>& itemList() const { return m_items; }

private:
  std::vector<Item> m_items;
};
