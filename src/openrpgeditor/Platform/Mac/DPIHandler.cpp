#include "Core/DPIHandler.hpp"

#include <SDL2/SDL.h>
#include <imgui.h>

#include <cmath>

#include "Core/Debug/Instrumentor.hpp"
#include "Core/Settings.hpp"
#include "Core/Window.hpp"

namespace App {

float DPIHandler::get_ui_scale() { return Settings::instance()->uiScale; }

WindowSize DPIHandler::get_dpi_aware_window_size(const Window::Settings& settings) { return {settings.width, settings.height}; }

void DPIHandler::set_render_scale(SDL_Renderer* renderer) {
  constexpr int display_index{0};
  // @todo: This should be 72.0F on Mac, but it seems like it is not. I'm not
  //  sure why, but this works ¯\_(ツ)_/¯
  const float default_dpi{96.0F};
  float dpi{default_dpi};

  SDL_GetDisplayDPI(display_index, nullptr, &dpi, nullptr);
  auto scale{std::floor(dpi / default_dpi)};
  SDL_RenderSetScale(renderer, scale, scale);
}

void DPIHandler::set_global_font_scaling(ImGuiIO* io) {
  // io->FontGlobalScale = 1.0F / get_ui_scale();
}

} // namespace App
