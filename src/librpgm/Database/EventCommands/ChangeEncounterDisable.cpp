#include "Database/EventCommands/ChangeEncounterDisable.hpp"

ChangeEncounterDisableCommand::ChangeEncounterDisableCommand(const std::optional<int>& indent,
                                                             const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(access);
}

void ChangeEncounterDisableCommand::serializeParameters(nlohmann::json& out) const {
  out.push_back(access);
}

std::string ChangeEncounterDisableCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Encounter" +
         colon.data() + DecodeEnumName(access) + ColorFormatter::popColor();
}
