#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"

#include <format>

struct MovementWaitCommand final : IMovementRouteStep {
  MovementWaitCommand() = default;
  explicit MovementWaitCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~MovementWaitCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Wait_del_Movement; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int duration;
};
