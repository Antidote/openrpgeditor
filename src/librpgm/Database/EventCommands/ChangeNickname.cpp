#include "Database/EventCommands/ChangeNickname.hpp"
#include "Database/Database.hpp"

ChangeNicknameCommand::ChangeNicknameCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(actor);
  parameters[1].get_to(nick);
}

std::string ChangeNicknameCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Nickname" + colon.data() +
         db.actorNameOrId(actor) + ", " + nick + ColorFormatter::popColor();
}
