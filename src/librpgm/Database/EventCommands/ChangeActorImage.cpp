#include "Database/EventCommands/ChangeActorImage.hpp"
#include "Database/Database.hpp"

ChangeActorImageCommand::ChangeActorImageCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(actor);
  parameters[1].get_to(charPicture);
  parameters[2].get_to(charIndex);
  parameters[3].get_to(facePicture);
  parameters[4].get_to(faceIndex);
  parameters[5].get_to(battlerPicture);
}

void ChangeActorImageCommand::serializeParameters(nlohmann::json& out) const {
  out.push_back(actor);
  out.push_back(charPicture);
  out.push_back(charIndex);
  out.push_back(facePicture);
  out.push_back(faceIndex);
  out.push_back(battlerPicture);
}

std::string ChangeActorImageCommand::stringRep(const Database& db) const {
  std::string charString = db.imageText(charPicture, charIndex);
  std::string faceString = db.imageText(facePicture, faceIndex);
  std::string battlerString = db.imageText(battlerPicture);

  const auto actName = db.actorNameOrId(actor);
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Actor Images" +
         colon.data() + actName + ", " + charString + ", " + faceString + ", " + battlerString +
         ColorFormatter::popColor();
}
