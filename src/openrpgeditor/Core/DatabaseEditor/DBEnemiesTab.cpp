#include "Core/DatabaseEditor/DBEnemiesTab.hpp"
#include "Core/Application.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Enemies.hpp"
#include "imgui.h"
#include "imgui_internal.h"

#include "imgui.h"

DBEnemiesTab::DBEnemiesTab(Enemies& Enemies, DatabaseEditor* parent) : IDBEditorTab(parent), m_enemies(Enemies) {
  m_selectedEnemy = m_enemies.enemy(1);
  if (m_selectedEnemy) {
    m_traitsEditor.setTraits(&m_selectedEnemy->traits);
  }
}

void DBEnemiesTab::draw() {
  if (!m_characterButtonTexture) {
    m_characterButtonTexture.emplace();
    m_characterButtonTexture->setSize(340, 200);
    if (!m_characterSheet) {
      m_characterSheet.emplace(m_selectedEnemy->battlerName);
    }
    const auto& [uv0, uv1] = m_characterSheet->getRectForCharacter(m_selectedEnemy->battlerHue);
    const Point offset{static_cast<int>(uv0.x() * m_characterSheet.value().texture().width()), static_cast<int>(uv0.y() * m_characterSheet.value().texture().height())};
    m_characterButtonTexture->setTexturesToComposite({{m_characterSheet.value().texture(), {m_characterSheet.value().characterWidth(), m_characterSheet.value().characterHeight()}, offset}});
  }

  if (m_isDropConfirmed) {
    m_selectedEnemy->dropItems.at(m_dropIndex).dataId = m_item.at(m_dropIndex);
    m_selectedEnemy->dropItems.at(m_dropIndex).kind = m_dropSelection;
    m_selectedEnemy->dropItems.at(m_dropIndex).denominator = m_dropDenominator;
    m_isOpen = false;
    m_isDropConfirmed = false;
  }
  ImGui::BeginChild("#orpg_enemies_editor");
  {
    ImGui::BeginChild("##orpg_enemies_editor_enemies", ImVec2{250.f, 0} * App::DPIHandler::get_ui_scale(), 0, ImGuiWindowFlags_HorizontalScrollbar);
    {
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Enemies");
        ImGui::BeginChild("##orpg_enemies_editor_enemies_list", ImVec2{0, ImGui::GetContentRegionMax().y - (App::DPIHandler::scale_value(108))});
        {
          ImGui::BeginGroup();
          {
            for (auto& skill_ : m_enemies.enemies()) {
              if (skill_.id == 0) {
                continue;
              }

              char name[4096];
              snprintf(name, 4096, "%04i %s", skill_.id, skill_.name.c_str());
              if (ImGui::Selectable(name, &skill_ == m_selectedEnemy) || (ImGui::IsItemFocused() && m_selectedEnemy != &skill_)) {
                m_selectedEnemy = &skill_;
                m_traitsEditor.setTraits(&m_selectedEnemy->traits);
              }
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        char str[4096];
        snprintf(str, 4096, "Max Enemies %i", m_enemies.count());
        ImGui::SeparatorText(str);
        if (ImGui::Button("Change Max", ImVec2{ImGui::GetContentRegionMax().x - (App::DPIHandler::scale_value(8)), 0})) {
          m_changeIntDialogOpen = true;
          m_editMaxEnemies = m_enemies.count();
        }
      }
      ImGui::EndGroup();
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_enemies_editor_enemies_enemy_properties");
    {
      if (m_selectedEnemy) {
        ImGui::BeginChild("##orpg_enemies_enemy_panel_left", ImVec2{ImGui::GetContentRegionMax().x / 2, 0.f});
        {
          ImGui::BeginGroup();
          {

            ImGui::SeparatorText("General Settings");
            // Name
            ImGui::BeginGroup();
            {
              char name[4096];
              strncpy(name, m_selectedEnemy->name.c_str(), 4096);
              if (ImGui::LabelOverLineEdit("##orpg_enemies_editor_name", "Name:", name, 4096, (ImGui::GetContentRegionMax().x / 2))) {
                m_selectedEnemy->name = name;
              }
            }
            ImGui::EndGroup();
            // Image
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::TextUnformatted("Image:");
              if (ImGui::ImageButtonEx(ImGui::GetID("##orpg_database_enemies_image"), m_characterButtonTexture->get(), ImVec2(m_characterButtonTexture->size()) * App::DPIHandler::get_ui_scale(),
                                       {0.f, 0.f}, {1.f, 1.f}, {}, {1.f, 1.f, 1.f, 1.f}, ImGuiButtonFlags_PressedOnDoubleClick)) {
                m_currentSheet = &m_characterSheet.value();
                // m_characterPicker->setCharacterInfo("", m_selectedItem->iconIndex);
                // m_characterPicker->setOpen(true);
              }
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Text("Max HP:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              if (ImGui::InputInt("##orpg_database_enemy_maxhp", &m_selectedEnemy->params.at(0), 1, 100)) {
                if (m_selectedEnemy->params.at(0) < -999)
                  m_selectedEnemy->params.at(0) = -999;
                if (m_selectedEnemy->params.at(0) > 999)
                  m_selectedEnemy->params.at(0) = 999;
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("Max MP:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              if (ImGui::InputInt("##orpg_database_enemy_maxmp", &m_selectedEnemy->params.at(1), 1, 100)) {
                if (m_selectedEnemy->params.at(1) < -999)
                  m_selectedEnemy->params.at(1) = -999;
                if (m_selectedEnemy->params.at(1) > 999)
                  m_selectedEnemy->params.at(1) = 999;
              }
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Text("Attack:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              if (ImGui::InputInt("##orpg_database_enemy_attack", &m_selectedEnemy->params.at(2), 1, 100)) {
                if (m_selectedEnemy->params.at(2) < -999)
                  m_selectedEnemy->params.at(2) = -999;
                if (m_selectedEnemy->params.at(2) > 999)
                  m_selectedEnemy->params.at(2) = 999;
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("Defense:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              if (ImGui::InputInt("##orpg_database_enemy_defense", &m_selectedEnemy->params.at(3), 1, 100)) {
                if (m_selectedEnemy->params.at(3) < -999)
                  m_selectedEnemy->params.at(3) = -999;
                if (m_selectedEnemy->params.at(3) > 999)
                  m_selectedEnemy->params.at(3) = 999;
              }
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Text("M.Attack:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              if (ImGui::InputInt("##orpg_database_enemy_mattack", &m_selectedEnemy->params.at(4), 1, 100)) {
                if (m_selectedEnemy->params.at(4) < -999)
                  m_selectedEnemy->params.at(4) = -999;
                if (m_selectedEnemy->params.at(4) > 999)
                  m_selectedEnemy->params.at(4) = 999;
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("M.Defense:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              if (ImGui::InputInt("##orpg_database_enemy_mdefense", &m_selectedEnemy->params.at(5), 1, 100)) {
                if (m_selectedEnemy->params.at(5) < -999)
                  m_selectedEnemy->params.at(5) = -999;
                if (m_selectedEnemy->params.at(5) > 999)
                  m_selectedEnemy->params.at(5) = 999;
              }
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Text("Agility:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              if (ImGui::InputInt("##orpg_database_enemy_agility", &m_selectedEnemy->params.at(6), 1, 100)) {
                if (m_selectedEnemy->params.at(6) < -9999)
                  m_selectedEnemy->params.at(6) = -9999;
                if (m_selectedEnemy->params.at(6) > 9999)
                  m_selectedEnemy->params.at(6) = 9999;
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("Luck:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              if (ImGui::InputInt("##orpg_database_enemy_luck", &m_selectedEnemy->params.at(7), 1, 100)) {
                if (m_selectedEnemy->params.at(7) < -9999)
                  m_selectedEnemy->params.at(7) = -9999;
                if (m_selectedEnemy->params.at(7) > 9999)
                  m_selectedEnemy->params.at(7) = 9999;
              }
            }
            ImGui::EndGroup();
            ImGui::SeparatorText("Rewards");
            ImGui::BeginGroup();
            {
              ImGui::Text("EXP:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              if (ImGui::InputInt("##orpg_database_enemy_exp", &m_selectedEnemy->exp, 1, 100)) {
                if (m_selectedEnemy->exp < 0)
                  m_selectedEnemy->exp = 0;
                if (m_selectedEnemy->exp > 9999999)
                  m_selectedEnemy->exp = 9999999;
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("Gold:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              if (ImGui::InputInt("##orpg_database_enemy_gold", &m_selectedEnemy->gold, 1, 100)) {
                if (m_selectedEnemy->gold < 0)
                  m_selectedEnemy->gold = 0;
                if (m_selectedEnemy->gold > 9999999)
                  m_selectedEnemy->gold = 9999999;
              }
            }
            ImGui::EndGroup();
            ImGui::Text("Drop Items");
            // Drop Item 1
            ImGui::BeginGroup();
            {
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(160));
              ImGui::PushID("##orpg_database_enemy_drops_1");
              if (ImGui::Button(m_selectedEnemy->dropItems.at(0).kind > 0 ? GetDropString(0, m_selectedEnemy->dropItems.at(0).kind).c_str() : "", ImVec2{ImGui::GetContentRegionMax().x, 0})) {
                m_dropIndex = 0;
                m_isOpen = true;
              }
              ImGui::PopID();
            }
            ImGui::EndGroup();
            // Drop Item 2
            ImGui::BeginGroup();
            {
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(160));
              ImGui::PushID("##orpg_database_enemy_drops_2");
              if (ImGui::Button(m_selectedEnemy->dropItems.at(1).kind > 0 ? GetDropString(1, m_selectedEnemy->dropItems.at(1).kind).c_str() : "", ImVec2{ImGui::GetContentRegionMax().x, 0})) {
                m_dropIndex = 1;
                m_isOpen = true;
              }
              ImGui::PopID();
            }
            ImGui::EndGroup();
            // Drop Item 3
            ImGui::BeginGroup();
            {
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(160));
              ImGui::PushID("##orpg_database_enemy_drops_3");
              if (ImGui::Button(m_selectedEnemy->dropItems.at(2).kind > 0 ? GetDropString(2, m_selectedEnemy->dropItems.at(2).kind).c_str() : "", ImVec2{ImGui::GetContentRegionMax().x, 0})) {
                m_dropIndex = 2;
                m_isOpen = true;
              }
              ImGui::PopID();
            }
            ImGui::EndGroup();
            // Action Patterns
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("##orpg_weapons_weapon_panel_right");
        {

          m_traitsEditor.draw(m_parent);
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("Note:");
            char note[8192];
            strncpy(note, m_selectedEnemy->note.c_str(), IM_ARRAYSIZE(note));
            if (ImGui::InputTextMultiline("##orpg_database_enemies_note", note, IM_ARRAYSIZE(note),
                                          ImVec2{ImGui::GetContentRegionMax().x - App::DPIHandler::scale_value(16), ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(16)})) {
              m_selectedEnemy->note = note;
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
      }

      ImGui::EndChild();
    }
    ImGui::EndChild();

    if (m_changeIntDialogOpen) {
      if (ImGui::Begin("Change Max Enemies", &m_changeIntDialogOpen,
                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
        ImGui::Text(
            "Specify an amount to resize the enemy list to\n"
            "This cannot be undone!");
        ImGui::InputInt("##value_input", &m_editMaxEnemies);
        ImGui::SameLine();
        if (ImGui::Button("Accept")) {
          m_changeConfirmDialogOpen = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
          m_changeIntDialogOpen = false;
        }
      }
      ImGui::End();

      if (m_changeConfirmDialogOpen) {
        if (ImGui::Begin("Confirm Change", &m_changeConfirmDialogOpen,
                         ImGuiWindowFlags_NoResize | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
          ImGui::Text("Are you sure?");
          if (ImGui::Button("Yes")) {
            int tmpId = m_selectedEnemy->id;
            m_enemies.resize(m_editMaxEnemies);
            m_selectedEnemy = m_enemies.enemy(tmpId);
            m_changeIntDialogOpen = false;
            m_changeConfirmDialogOpen = false;
          }
          ImGui::SameLine();
          if (ImGui::Button("Cancel")) {
            m_changeIntDialogOpen = false;
            m_changeConfirmDialogOpen = false;
          }
        }
        ImGui::End();
      }
    }
    drawPopup();
  }
}
void DBEnemiesTab::drawPopup() {
  if (m_isOpen) {
    ImGui::OpenPopup("Drop Item Selection");
  }

  if (ImGui::BeginPopupModal("Drop Item Selection", nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize)) {
    if (item_picker) {
      auto [closed, confirmed] = item_picker->draw();
      if (closed) {
        if (confirmed) {
          m_item.at(0) = item_picker->selection();
        }
        item_picker.reset();
      }
    }
    if (weapon_picker) {
      auto [closed, confirmed] = weapon_picker->draw();
      if (closed) {
        if (confirmed) {
          m_item.at(1) = weapon_picker->selection();
        }
        weapon_picker.reset();
      }
    }

    if (armor_picker) {
      auto [closed, confirmed] = armor_picker->draw();
      if (closed) {
        if (confirmed) {
          m_item.at(2) = armor_picker->selection();
        }
        armor_picker.reset();
      }
    }
    ImGui::BeginChild("##orpg_dropitem_left_child", ImVec2(0, 0), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize, ImGuiWindowFlags_NoBackground);
    {

      if (ImGui::RadioButton("None", m_dropSelection == 0)) {
        m_dropSelection = 0;
      }
      if (ImGui::RadioButton("Item", m_dropSelection == 1)) {
        m_dropSelection = 1;
      }
      if (ImGui::RadioButton("Weapon", m_dropSelection == 2)) {
        m_dropSelection = 2;
      }
      if (ImGui::RadioButton("Armor", m_dropSelection == 3)) {
        m_dropSelection = 3;
      }
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_dropitem_right_child", ImVec2(0, 0), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize, ImGuiWindowFlags_NoBackground);
    {
      // None
      ImGui::NewLine();
      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.f);
      //  Item
      ImGui::BeginDisabled(m_dropSelection != 1);
      {
        ImGui::PushID("##orpg_dropitem_item");
        if (ImGui::Button(m_dropSelection != 1 ? "" : Database::instance()->itemNameOrId(m_item.at(0)).c_str(), ImVec2{200 - (15 * App::DPIHandler::get_ui_scale()), 0})) {
          item_picker = ObjectPicker<Item>("Item"sv, Database::instance()->items.items(), m_item.at(0));
          item_picker->setOpen(true);
        }
        ImGui::PopID();
      }
      ImGui::EndDisabled();
      // Weapon
      ImGui::BeginDisabled(m_dropSelection != 2);
      {
        ImGui::PushID("##orpg_dropitem_weapon");
        if (ImGui::Button(m_dropSelection != 2 ? "" : Database::instance()->weaponNameOrId(m_item.at(1)).c_str(), ImVec2{200 - (15 * App::DPIHandler::get_ui_scale()), 0})) {
          weapon_picker = ObjectPicker<Weapon>("Weapon"sv, Database::instance()->weapons.weaponList(), m_item.at(1));
          weapon_picker->setOpen(true);
        }
        ImGui::PopID();
      }
      ImGui::EndDisabled();
      // Armor
      ImGui::BeginDisabled(m_dropSelection != 3);
      {
        ImGui::PushID("##orpg_dropitem_armor");
        if (ImGui::Button(m_dropSelection != 3 ? "" : Database::instance()->armorNameOrId(m_item.at(2)).c_str(), ImVec2{200 - (15 * App::DPIHandler::get_ui_scale()), 0})) {
          armor_picker = ObjectPicker<Armor>("Armor"sv, Database::instance()->armors.armorList(), m_item.at(2));
          armor_picker->setOpen(true);
        }
        ImGui::PopID();
      }
      ImGui::EndDisabled();
      ImGui::BeginDisabled(m_dropSelection < 1);
      {
        ImGui::SeparatorText("Probability");
        if (ImGui::InputInt("##orpg_dropitem_denom", &m_dropDenominator)) {
          if (m_dropDenominator < 1) {
            m_dropDenominator = 1;
          }
          if (m_dropDenominator > 1000) {
            m_dropDenominator = 1000;
          }
        }
      }
      ImGui::EndDisabled();
    }
    ImGui::EndChild();
    ImGui::Separator();
    ImGui::BeginGroup();
    {
      if (ImGui::Button("OK")) {
        m_isOpen = false;
        m_isDropConfirmed = true;
        ImGui::CloseCurrentPopup();
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel")) {
        m_isOpen = false;
        ImGui::CloseCurrentPopup();
      }
      ImGui::EndGroup();
    }
    ImGui::EndPopup();
  }
}
std::string DBEnemiesTab::GetDropString(int dropIndex, int kindId) {
  switch (kindId) {
  case 1:
    return Database::instance()->itemNameOrId(m_selectedEnemy->dropItems.at(dropIndex).dataId);
  case 2:
    return Database::instance()->weaponNameOrId(m_selectedEnemy->dropItems.at(dropIndex).dataId);
  case 3:
    return Database::instance()->armorNameOrId(m_selectedEnemy->dropItems.at(dropIndex).dataId);
  default:
    break;
  }
}