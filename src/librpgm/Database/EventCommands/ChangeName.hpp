#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeNameCommand final : IEventCommand {
  ChangeNameCommand() = default;
  explicit ChangeNameCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ChangeNameCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Name; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int actor = 1;
  std::string name;

};
