#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct NextScriptCommand : IEventCommand {
  ~NextScriptCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Next_Script; }
  void setNextString(const std::string& str) override { script = str; }
  std::string script;
};

struct ScriptCommand : IEventCommand {
  ~ScriptCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Script; }
  std::string script;
  std::vector<std::shared_ptr<NextScriptCommand>> moreScript;
  [[nodiscard]] std::string stringRep(const Database& db) const override {
    std::string ret = indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
  + "Script " + colon.data()  + script;
    for (const auto& t : moreScript) {
      if (!ret.empty()) {
        ret += "\n";
      }
      ret += indentText(indent) + colon.data() + indentText(1) + indentText(1) + colon.data() + ColorFormatter::getColorCode(code()) + t->script;
    }

    return ret;
  }
};
