#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementChangeImageCommand : IEventCommand {
  ~MovementChangeImageCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Image; }

  std::string image;
  int character;

  void serializeParameters(nlohmann::json& out) override {
    out.push_back(image);
    out.push_back(character);
  }

  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Image : " + image + " (" +  std::to_string(character) + ")" + ColorFormatter::popColor();
  }
};