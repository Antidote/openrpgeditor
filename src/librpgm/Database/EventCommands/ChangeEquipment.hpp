#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeEquipmentCommand : IEventCommand {
  ~ChangeEquipmentCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Equipment; }

  int actorId;
  int equipType;
  int equipment;

  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Equipment : {}, {} = " + (equipment > 0 ? "{}" : "None") + ColorFormatter::popColor();
  }
};