#pragma once

#include "Core/Graphics/CharacterSheet.hpp"

#include "Database/Event.hpp"

struct ImGuiWindow;
struct MapEditor;
struct MapEvent {
  MapEvent() = delete;
  MapEvent(MapEditor* parent, Event* event);

  void draw(float mapScale, bool isHovered, bool selected, bool halfAlpha, ImGuiWindow* win);

private:
  MapEditor* m_parent;
  Event* m_event;
  CharacterSheet m_characterSheet;
  int m_pattern{0};
};
