#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct NextTextCommand : IEventCommand {
  ~NextTextCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Next_Text; }
  void serializeParameters(nlohmann::json& out) override {
    out.push_back(text);
  }

  std::string text;
};

struct ShowTextCommand : IEventCommand {
  ~ShowTextCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Text; }
  [[nodiscard]] std::string stringRep(const Database& db) const override {
    std::string ret = indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Text " + colon.data() + ColorFormatter::popColor() +
      (faceImage == "" ? "None, " : faceImage + std::format("({}), ", faceIndex)) + DecodeEnumName(background) + ", " + DecodeEnumName(position);

    for (const auto& t : text) {
      if (!ret.empty()) {
        ret += "\n";
      }
      ret += indentText(indent) + colon.data() + indentText(1) + indentText(1) + colon.data() + ColorFormatter::getColorCode(code()) + t->text;
    }
    return ret;
  }

  void serializeParameters(nlohmann::json& out) override {
    out.push_back(faceImage);
    out.push_back(faceIndex);
    out.push_back(background);
    out.push_back(position);
  }

  std::string faceImage;
  int faceIndex;
  TextBackground background;
  TextWindowPosition position;
  std::string textLine;
  std::vector<std::shared_ptr<NextTextCommand>> text;
};