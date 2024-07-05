#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeEXPCommand final : IEventCommand {
  ChangeEXPCommand() = default;
  explicit ChangeEXPCommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~ChangeEXPCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_EXP; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  ActorComparisonSource comparison;
  int value;
  QuantityChangeOp quantityOp;
  QuantityChangeSource quantitySource;
  int quantity;
  bool showLevelUp;

};