#pragma once

#include "nlohmann/json.hpp"
#include <memory>

struct IEventCommand;
struct MovementRoute {
  friend void to_json(nlohmann::json& to, const MovementRoute& route);
  friend void from_json(const nlohmann::json& to, MovementRoute& route);

  std::vector<std::shared_ptr<IEventCommand>> list;
  bool repeat;
  bool skippable;
  bool wait;
};