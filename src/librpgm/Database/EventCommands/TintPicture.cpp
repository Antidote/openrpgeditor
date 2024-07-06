#include "Database/EventCommands/TintPicture.hpp"

TintPictureCommand::TintPictureCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(picture);
  auto colorValues = parameters[1];
  colorValues[0].get_to(color.r);
  colorValues[1].get_to(color.g);
  colorValues[2].get_to(color.b);
  colorValues[3].get_to(color.gray);
  parameters[2].get_to(duration);
  parameters[3].get_to(waitForCompletion);
}

void TintPictureCommand::serializeParameters(nlohmann::json& out) const {
  out.push_back(picture);
  auto colorValues = nlohmann::json();
  colorValues.push_back(color.r);
  colorValues.push_back(color.g);
  colorValues.push_back(color.b);
  colorValues.push_back(color.gray);
  out.push_back(colorValues);
  out.push_back(duration);
  out.push_back(waitForCompletion);
}

std::string TintPictureCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Tint Picture" + colon.data() +
         std::format("#{}, ({},{},{},{}), {} frames", picture, color.r, color.g, color.b, color.gray, duration) +
         ColorFormatter::popColor() +
         (waitForCompletion == true ? ColorFormatter::getColor(Color::Gray) + " (Wait)" + ColorFormatter::popColor()
                                    : "");
}
