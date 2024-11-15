#include "Database/EventCommands/MovementRoute/Wait.hpp"

MovementWaitCommand::MovementWaitCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IMovementRouteStep(indent, parameters) {
  parameters.at(0).get_to(duration);
}

void MovementWaitCommand::serializeParameters(nlohmann::json& out) const { out.push_back(duration); }

std::string MovementWaitCommand::stringRep(const Database& db) const {
  return symbol(code()) + std::string("Wait") + colon.data() + std::to_string(duration) + " frames";
}
