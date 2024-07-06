#include "Database/EventCommands/ChangeTransparency.hpp"

ChangeTransparencyCommand::ChangeTransparencyCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(transparency);
}

void ChangeTransparencyCommand::serializeParameters(nlohmann::json& out) const { out.push_back(transparency); }

std::string ChangeTransparencyCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Transparency" +
         colon.data() + DecodeEnumName(transparency) + ColorFormatter::popColor();
}