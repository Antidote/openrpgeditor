#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include "Database/Audio.hpp"
#include <format>

struct ChangeDefeatMECommand final : IEventCommand {
  ChangeDefeatMECommand() = default;
  explicit ChangeDefeatMECommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ChangeDefeatMECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Defeat_ME; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  Audio me;
};