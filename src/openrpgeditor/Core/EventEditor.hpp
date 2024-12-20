#pragma once

#include "Core/EventEditor/EVPage.hpp"
#include "Core/TemplateEditor/Dialog/TemplatesEvent.hpp"
#include "Database/Event.hpp"

#include <vector>

struct Project;

struct EventEditor {
  EventEditor(Project* parent, Event* event) : m_parent(parent), m_event(event) {
    for (auto& page : event->pages) {
      m_pages.emplace_back(this, &page);
    }
    m_id = event->id;
  }
  void fixupPages();

  bool draw();

  Event* event() { return m_event; }
  const Event* event() const { return m_event; }

  Project* project() { return m_parent; }
  const Project* project() const { return m_parent; }

  int id() const { return m_id; }

private:
  std::optional<ObjectPicker<Template>> template_picker;

  Project* m_parent = nullptr;
  Event* m_event = nullptr;
  int m_id;
  std::vector<EVPage> m_pages;
  int m_selectedPage = 0;
  bool m_isOpen = true;
};
