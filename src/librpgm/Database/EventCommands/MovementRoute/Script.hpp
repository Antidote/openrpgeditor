#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementScriptCommand : IEventCommand {
  ~MovementScriptCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Script_del_Movement; }
  std::string script;
  void serializeParameters(nlohmann::json& out) override {
    out.push_back(script);
  }

  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Script : " + script + ColorFormatter::popColor();
  }
};