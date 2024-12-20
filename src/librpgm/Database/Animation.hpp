#pragma once

#include "Database/Audio.hpp"
#include "Database/Globals.hpp"

#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <vector>

class Animation {
public:
  friend void to_json(nlohmann::ordered_json& to, const Animation& animation);
  friend void from_json(const nlohmann::ordered_json& from, Animation& animation);
  struct Color {
    int r;
    int g;
    int b;
    int intensity;
  };

  struct FramePart {
    int pattern;
    int x;
    int y;
    int scale;
    int rotation;
    Mirror mirror;
    int opacity;
    Blend blend;
  };

  struct Timing {
    void to_json(nlohmann::ordered_json& to, const Timing& timing);
    void from_json(const nlohmann::ordered_json& from, Timing& timing);
    std::array<int, 4> flashColor{};
    int flashDuration{};
    FlashScope flashScope = FlashScope{};
    int frame{};
    std::optional<Audio> se{};
  };

  int id() const { return m_id; }
  void setId(const int id) { m_id = id; }

  const std::string& name() const { return m_name; }
  void setName(const std::string& name) { m_name = name; }

  int m_id = 0;
  int m_animation1Hue = 0;
  std::string m_animation1Name;
  int m_animation2Hue = 0;
  std::string m_animation2Name;
  std::vector<std::vector<std::array<int, 8>>> m_frames;
  std::string m_name;
  Position m_position = Position::Head;
  std::vector<Timing> m_timings;

  /*!
   * @name m_isValid
   * @details
   * Indicates that this is an actual valid entry and not a dummy
   * When making a new entry make sure to set this to true or it won't be
   * serialized.
   */
  bool m_isValid{false};
};
void to_json(nlohmann::ordered_json& to, const Animation::Timing& timing);
void from_json(const nlohmann::ordered_json& from, Animation::Timing& timing);
void to_json(nlohmann::ordered_json& to, const Animation& animation);
void from_json(const nlohmann::ordered_json& from, Animation& animation);