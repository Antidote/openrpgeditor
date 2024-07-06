#include "Database/EventCommands/ControlSelfSwitch.hpp"

ControlSelfSwitchCommand::ControlSelfSwitchCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(selfSw);
  parameters[1].get_to(turnOff);
}

void ControlSelfSwitchCommand::serializeParameters(nlohmann::json& out) const {
  out.push_back(selfSw);
  out.push_back(turnOff);
}

std::string ControlSelfSwitchCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Control Self Switch" +
         colon.data() + selfSw + " = " + DecodeEnumName(turnOff) + ColorFormatter::popColor();
}
