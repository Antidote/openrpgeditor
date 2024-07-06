#include "Database/EventCommands/ErasePicture.hpp"

ErasePictureCommand::ErasePictureCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(picture);
}

void ErasePictureCommand::serializeParameters(nlohmann::json& out) const { out.push_back(picture); }

std::string ErasePictureCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Erase Picture" + colon.data() +
         std::to_string(picture) + ColorFormatter::popColor();
}
