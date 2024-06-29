#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeMPCommand : IEventCommand {
  ~ChangeMPCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_MP; }

  ActorComparisonSource comparison;
  int value;
  QuantityChangeOp quantityOp;
  QuantityChangeSource quantitySource;
  int quantity;

  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change MP : {}, " + DecodeEnumName(quantityOp) + (quantitySource == QuantityChangeSource::Variable ? " {} " : std::to_string(quantity)) + ColorFormatter::popColor();
  }
};