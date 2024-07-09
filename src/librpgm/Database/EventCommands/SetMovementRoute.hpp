#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include "Database/MovementRoute.hpp"
#include <format>

struct MovementRouteStepCommand final : IEventCommand {
  MovementRouteStepCommand() = default;
  explicit MovementRouteStepCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~MovementRouteStepCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Movement_Route_Step; }
  std::shared_ptr<IEventCommand> step;

  void serializeParameters(nlohmann::json& out) const override;

  [[nodiscard]] std::string stringRep(const Database& db) const override { return step->stringRep(db); }
};

struct SetMovementRouteCommand final : IEventCommand {
  SetMovementRouteCommand() = default;
  explicit SetMovementRouteCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~SetMovementRouteCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Set_Movement_Route; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  void addStep(MovementRouteStepCommand* step) { editNodes.emplace_back(step); }

  int character{0};
  MovementRoute route;
  std::vector<std::shared_ptr<MovementRouteStepCommand>> editNodes;
};