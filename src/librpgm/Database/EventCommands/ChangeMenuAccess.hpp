#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeMenuAccessCommand final : IEventCommand {
  ChangeMenuAccessCommand() = default;
  explicit ChangeMenuAccessCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ChangeMenuAccessCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Menu_Access; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  AccessMode access;
};