#include "Database/EventCommands/ChangeMenuAccess.hpp"

ChangeMenuAccessCommand::ChangeMenuAccessCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(access);
}

void ChangeMenuAccessCommand::serializeParameters(nlohmann::json& out) const {
  out.push_back(access);
}

std::string ChangeMenuAccessCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Menu Access" +
         colon.data() + DecodeEnumName(access) + ColorFormatter::popColor();
}
