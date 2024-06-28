#pragma once
#include "Database/Globals.hpp"
#include "Database/ColorFormatter.hpp"
#include "nlohmann/json.hpp"


struct IEventCommand {
  std::optional<int> indent{};
  mutable bool m_isDirty{false};

  static constexpr std::string_view diamond = "\u25c6";
  static constexpr std::string_view colon = "\uff1a";
  virtual ~IEventCommand() = default;
  [[nodiscard]] virtual EventCode code() const = 0;
  [[nodiscard]] virtual bool isDirty() const { return m_isDirty; };
  [[nodiscard]] virtual std::string stringRep() const {
    return std::string(indent ? *indent * 4 : 0, ' ') + DecodeEnumName(code());
  }
  virtual std::string symbol(EventCode code) const {
    return static_cast<int>(code) < 400 ? diamond.data() : colon.data();
  }
  virtual std::string indentText(std::optional<int> indent) const {
    std::string text = "";
    for(int i = 0; i < indent; ++i) {
      text += "  ";
    }
    return text;
  }
};