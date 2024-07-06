#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct TransferPlayerCommand final : IEventCommand {
  TransferPlayerCommand() = default;
  explicit TransferPlayerCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~TransferPlayerCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Transfer_Player; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  TransferMode mode;
  int mapId;
  int x;
  int y;
  Direction direction;
  Fade fade;

};
