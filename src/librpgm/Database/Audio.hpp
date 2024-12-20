#pragma once

#include <array>
#include <string>
#include <string_view>
#include <vector>

#include "nlohmann/json.hpp"

struct Audio {
  friend void to_json(nlohmann::ordered_json& out, const Audio& audio);
  friend void from_json(const nlohmann::ordered_json& from, Audio& audio);
  std::string name; // base name of the file (without extension) in audio/se
  int pan = 0;
  int pitch = 100;
  int volume = 100;
};
void to_json(nlohmann::json& out, const Audio& audio);
void from_json(const nlohmann::json& from, Audio& audio);