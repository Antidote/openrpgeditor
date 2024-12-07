#pragma once

#include "Database/Map.hpp"
#include "nlohmann/json.hpp"
#include <string>

class ISerializable;
struct MapInfo {
  friend void to_json(nlohmann::ordered_json& json, const MapInfo& mapinfo);
  friend void from_json(const nlohmann::ordered_json& json, MapInfo& mapinfo);
  bool expanded{};
  int id{};
  std::string name;
  int order{};
  int parentId{};
  double scrollX{};
  double scrollY{};

  std::vector<MapInfo*>& children() { return m_children; }
  const std::vector<MapInfo*>& children() const { return m_children; }

  Map* map() { return m_map.get(); }
  const Map* map() const { return m_map.get(); }

  Event* event(int id) { return m_map ? m_map->event(id) : nullptr; }
  const Event* event(int id) const { return m_map ? m_map->event(id) : nullptr; }

private:
  friend void recursiveSort(MapInfo& in);
  friend class MapInfos;
  friend class MapInfosSerializer;
  std::vector<MapInfo*> m_children;
  std::unique_ptr<Map> m_map;
};
void to_json(nlohmann::ordered_json& json, const MapInfo& mapinfo);
void from_json(const nlohmann::ordered_json& json, MapInfo& mapinfo);

class MapInfos {
public:
  static MapInfos load(std::string_view filepath);
  bool serialize(std::string_view filename);

  [[nodiscard]] MapInfo* map(int id) {
    for (auto& item : m_mapinfos) {
      if (item->id == id) {
        return &item.value();
      }
    }
    return nullptr;
  }

  [[nodiscard]] const MapInfo* map(int id) const {
    for (const auto& set : m_mapinfos) {
      if (set->id == id) {
        return &set.value();
      }
    }

    return nullptr;
  }

  bool empty() const { return m_mapinfos.empty(); }

  MapInfo& root() { return m_mapinfos[0].value(); }
  const MapInfo& root() const { return m_mapinfos[0].value(); }

  void buildTree(bool reset = false);

  std::vector<std::optional<MapInfo>>& mapInfos() { return m_mapinfos; }
  const std::vector<std::optional<MapInfo>>& mapInfos() const { return m_mapinfos; }

  void setCurrentMap(MapInfo* map) { m_currentMap = map; }
  MapInfo* currentMap() { return m_currentMap; }
  const MapInfo* currentMap() const { return m_currentMap; }

  void loadAllMaps();

  void rebuildOrdering();

private:
  std::vector<std::optional<MapInfo>> m_mapinfos;
  MapInfo* m_currentMap = nullptr;

  void mapLoadCallback(const std::shared_ptr<ISerializable>& data);
};
