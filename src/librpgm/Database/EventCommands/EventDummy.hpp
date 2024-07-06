#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include "Database/ColorFormatter.hpp"

struct EventDummy : IEventCommand {
  EventDummy() = default;
  explicit EventDummy(const std::optional<int>& _indent, const nlohmann::json& parameters)
  : IEventCommand(_indent, parameters) {}
  ~EventDummy() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Event_Dummy; }
  [[nodiscard]] std::string stringRep(const Database& db) const override { return indentText(indent) + symbol(code()); }
};