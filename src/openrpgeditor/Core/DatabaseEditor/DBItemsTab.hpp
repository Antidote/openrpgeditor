#pragma once
#include "Core/CommonUI/EffectsEditor.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Core/Graphics/CheckeredCompositeTexture.hpp"
#include "Core/Graphics/IconSheet.hpp"
#include "Database/Animation.hpp"
#include "Database/Items.hpp"

class Items;
class DBItemsTab final : public IDBEditorTab {
public:
  DBItemsTab() = delete;
  explicit DBItemsTab(Items& items, DatabaseEditor* parent);
  void draw() override;

  [[nodiscard]] std::vector<Item>& items() { return m_items.items(); }
  [[nodiscard]] const std::vector<Item>& items() const { return m_items.items(); }

  [[nodiscard]] Item* item(const int id) { return m_items.item(id); }
  [[nodiscard]] const Item* item(const int id) const { return m_items.item(id); }

private:
  Items& m_items;
  Item* m_selectedItem{};
  int m_editMaxItems{};
  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
  IconSheet* m_currentSheet = nullptr;
  std::optional<CheckeredCompositeTexture> m_itemButtonTexture;
  std::optional<IconSheet> m_itemSheet;
  std::optional<IconSheet> m_itemPicker;
  EffectsEditor m_effectsEditor;
  std::optional<ObjectPicker<Animation>> m_animationPicker;
};