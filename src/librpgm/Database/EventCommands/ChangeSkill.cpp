#include "Database/EventCommands/ChangeSkill.hpp"
#include "Database/Database.hpp"

ChangeSkillCommand::ChangeSkillCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(comparison);
  parameters[1].get_to(value);
  parameters[2].get_to(skillOp);
  parameters[3].get_to(skill);
}

std::string ChangeSkillCommand::stringRep(const Database& db) const {
  std::string actorName;
  if (comparison == ActorComparisonSource::Variable) {
    actorName = db.system.variable(value);
    actorName = actorName.empty() ? std::format("{{#{:04}}}", value) : actorName;
  } else {
    if (value == 0) {
      actorName = "Entire Party";
    } else {
      const auto actor = db.actors.actor(value);
      actorName = actor && !actor->name.empty() ? actor->name : std::format("#{:04}", value);
    }
  }

  const auto sk = db.states.state(skill);
  std::string skillName = sk && !sk->name.empty() ? sk->name : std::format("{{#{:04}}}", skill);
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Skill" + colon.data() +
         actorName + ", " + DecodeEnumName(skillOp) + " " + skillName + ColorFormatter::popColor();
}