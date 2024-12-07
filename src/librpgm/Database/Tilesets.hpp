#pragma once

#include <array>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

class Tileset {
public:
  friend void to_json(nlohmann::ordered_json& j, const Tileset& tileset);
  friend void from_json(const nlohmann::ordered_json& j, Tileset& tileset);

  enum class Mode {
    World,
    Area,
  };

  int id;
  std::vector<int> flags;
  Mode mode;
  std::string name;
  std::string note;
  std::array<std::string, 9> tilesetNames;

  /*!
   * @name m_isValid
   * @details
   * Indicates that this is an actual valid entry and not a dummy
   *
   * When making a new entry make sure to set this to true or it won't be
   * serialized.
   */
  bool m_isValid{false};
};
void to_json(nlohmann::json& j, const Tileset& tileset);
void from_json(const nlohmann::json& j, Tileset& tileset);

class Tilesets {
public:
  static Tilesets load(std::string_view filepath);
  bool serialize(std::string_view filepath);

  [[nodiscard]] Tileset* tileset(int id) {
    for (auto& item : m_tilesets) {
      if (item.id == id && item.m_isValid) {
        return &item;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const Tileset* tileset(int id) const {
    for (const auto& item : m_tilesets) {
      if (item.id == id && item.m_isValid) {
        return &item;
      }
    }

    return nullptr;
  }

  [[nodiscard]] std::vector<Tileset>& tilesets() { return m_tilesets; }
  [[nodiscard]] const std::vector<Tileset>& tilesets() const { return m_tilesets; }
  int count() { return m_tilesets.size() - 1; }

  void resize(int newSize) {
    ++newSize;
    int oldSize = m_tilesets.size();
    m_tilesets.resize(newSize);
    if (newSize > oldSize) {
      for (int i = oldSize; i < m_tilesets.size(); ++i) {
        m_tilesets[i].id = i;
      }
    }
  }

private:
  std::vector<Tileset> m_tilesets;
};
