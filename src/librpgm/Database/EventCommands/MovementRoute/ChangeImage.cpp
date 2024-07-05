#include "Database/EventCommands/MovementRoute/ChangeImage.hpp"

MovementChangeImageCommand::MovementChangeImageCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IMovementRouteStep(indent, parameters) {
  parameters[0].get_to(image);
  parameters[1].get_to(character);
}

void MovementChangeImageCommand::serializeParameters(nlohmann::json& out) {
  out.push_back(image);
  out.push_back(character);
}

std::string MovementChangeImageCommand::stringRep(const Database& db) const {
  return whiteDiamond.data() + std::string("Image") + colon.data() + db.imageText(image) + " " +
         db.parentheses(std::to_string(character));
}
