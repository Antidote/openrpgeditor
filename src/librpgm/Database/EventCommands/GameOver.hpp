#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct GameOverCommand final : IEventCommand {
  GameOverCommand() = default;
  explicit GameOverCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {}
  ~GameOverCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Game_Over; }
};