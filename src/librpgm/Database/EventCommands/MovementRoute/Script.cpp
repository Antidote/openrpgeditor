#include "Database/EventCommands/MovementRoute/Script.hpp"

MovementScriptCommand::MovementScriptCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IMovementRouteStep(indent, parameters) {
  parameters[0].get_to(script);
}

void MovementScriptCommand::serializeParameters(nlohmann::json& out) { out.push_back(script); }

std::string MovementScriptCommand::stringRep(const Database& db) const {
  return whiteDiamond.data() + DecodeEnumName(code()) + colon.data() + script;
}
