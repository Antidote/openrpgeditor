#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct StopSECommand final : IEventCommand {
  StopSECommand() = default;
  explicit StopSECommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {}
  ~StopSECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Stop_SE; }
};
