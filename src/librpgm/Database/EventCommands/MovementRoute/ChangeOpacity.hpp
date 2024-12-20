#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementChangeOpacityCommand final : IMovementRouteStep {
  MovementChangeOpacityCommand() = default;
  explicit MovementChangeOpacityCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~MovementChangeOpacityCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Opacity; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int opacity{255};
};
