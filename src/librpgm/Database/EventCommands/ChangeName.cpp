#include "Database/EventCommands/ChangeName.hpp"
#include "Database/Database.hpp"

ChangeNameCommand::ChangeNameCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(actor);
  parameters[1].get_to(name);
}

std::string ChangeNameCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Name" + colon.data() +
         db.actorNameOrId(actor) + ", " + name + ColorFormatter::popColor();
}
