#pragma once
#include "Core/CommonUI/EventCommandEditor.hpp"
#include "Core/CharacterSheet.hpp"

#include "Database/EventPage.hpp"

struct EventEditor;
struct EventPage;
struct EVPage {
  EVPage(EventEditor* parent, EventPage* page);

  /* Returns true when closed, closing is the equivelant of deleting */
  std::tuple<bool, bool> draw(bool canDelete, int index);

  void clearPage();

  void setPage(EventPage* page) {
    m_page = page;
    m_commandEditor.setCommands(&m_page->list);
  }

  void setParent(EventEditor* parent) { m_parent = parent; }

private:
  EventEditor* m_parent;
  EventPage* m_page;
  EventCommandEditor m_commandEditor;
  CharacterSheet m_characterSheet;
};