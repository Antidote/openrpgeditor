﻿#include "Database/EventCommands/IEventCommand.hpp"

IEventCommand::IEventCommand(const std::optional<int>& _indent, [[maybe_unused]] const nlohmann::ordered_json& parameters) : m_indent(_indent) {}

void IEventCommand::serialize(nlohmann::ordered_json& out, const bool doIndent, const bool doParameters, bool oldValues) const {
  out["code"] = code();

  if (doIndent) {
    out["indent"] = indent();
  }

  if (doParameters) {
    out["parameters"] = nlohmann::json::array();
    serializeParameters(out["parameters"]);
  }
}

std::string IEventCommand::stringRep(const Database& db) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code()) + DecodeEnumName(code()) + ColorFormatter::popColor();
}

std::string IEventCommand::symbol(EventCode code) const { return static_cast<int>(code) < 400 ? diamond.data() : colon.data(); }

std::string IEventCommand::indentText(std::optional<int> indent) const {
  std::string text;
  for (int i = 0; i < indent; ++i) {
    text += "  ";
  }
  return text;
}
