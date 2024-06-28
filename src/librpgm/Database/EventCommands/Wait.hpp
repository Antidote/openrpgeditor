#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct WaitCommand : IEventCommand {
  int duration;
  ~WaitCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Wait; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Wait : " + std::to_string(duration) + " frames";
  }
};
