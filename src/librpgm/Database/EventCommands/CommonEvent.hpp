#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct CommonEventCommand final : IEventCommand {
  CommonEventCommand() = default;
  explicit CommonEventCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~CommonEventCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Common_Event; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<CommonEventCommand>(*this); }

  int event = 1;
};
