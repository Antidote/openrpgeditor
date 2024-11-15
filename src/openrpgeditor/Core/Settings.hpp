#pragma once

#include "Database/Globals.hpp"
#include "nlohmann/json.hpp"

#include <deque>

struct Settings {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Settings, window, mru, lastDirectory, uiScale, fontSize, monoFontSize,
                                              currentNWJSVersion);
  struct WindowRect {
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(WindowRect, x, y, w, h, maximized);
    int x{};
    int y{};
    int w{1920};
    int h{1080};
    bool maximized{false};
  };

  Settings();
  bool load(std::string_view path);
  void serialize(std::string_view path);
  WindowRect window{};
  std::deque<std::pair<std::string, std::string>> mru;
  std::string lastDirectory;
  float uiScale{1.0};
  float fontSize{12.f};
  float monoFontSize{12.f};
  std::string currentNWJSVersion;

  [[nodiscard]] static Settings* instance() { return m_instance; }

private:
  static Settings* m_instance;
};