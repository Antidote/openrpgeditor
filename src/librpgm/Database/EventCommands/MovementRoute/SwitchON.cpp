#include "Database/EventCommands/MovementRoute/SwitchON.hpp"

MovementSwitchONCommand::MovementSwitchONCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IMovementRouteStep(indent, parameters) {
  parameters.at(0).get_to(id);
}

void MovementSwitchONCommand::serializeParameters(nlohmann::json& out) const { out.push_back(id); }

std::string MovementSwitchONCommand::stringRep(const Database& db) const {
  return symbol(code()) + DecodeEnumName(code()) + colon.data() + std::format("{:04}", id);
}
