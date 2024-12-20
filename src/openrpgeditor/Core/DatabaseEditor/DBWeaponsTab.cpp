#include "Core/DatabaseEditor/DBWeaponsTab.hpp"
#include "Core/Application.hpp"
#include "Database/Weapons.hpp"

#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "imgui.h"
#include "imgui_internal.h"

DBWeaponsTab::DBWeaponsTab(Weapons& weapons, DatabaseEditor* parent) : IDBEditorTab(parent), m_weapons(weapons) {
  m_selectedWeapon = m_weapons.weapon(1);
  if (m_selectedWeapon) {
    m_traitsEditor.setTraits(&m_selectedWeapon->traits());
  }
  m_maxWeapons = m_weapons.count();
}

void DBWeaponsTab::draw() {
  if (!m_itemButtonTexture) {
    m_itemButtonTexture.emplace();
    m_itemButtonTexture->setSize(38, 38);
    if (!m_itemSheet) {
      m_itemSheet.emplace(*m_parent->getIconSheet());
    }
    const auto& [uv0, uv1] = m_itemSheet.value().rectForId(m_selectedWeapon->iconIndex());
    const Point offset{static_cast<int>(uv0.x() * m_itemSheet.value().texture().width()), static_cast<int>(uv0.y() * m_itemSheet.value().texture().height())};
    m_itemButtonTexture->setTexturesToComposite({{m_itemSheet.value().texture(), {m_itemSheet.value().iconWidth(), m_itemSheet.value().iconHeight()}, offset}});
  }
  if (m_animationPicker) {
    if (const auto [closed, confirmed] = m_animationPicker->draw(); closed) {
      if (confirmed) {
        m_selectedWeapon->setAnimationId(m_animationPicker->selection());
      }
      m_animationPicker.reset();
    }
  }
  ImGui::BeginChild("#orpg_weapons_editor");
  {
    ImGui::BeginChild("##orpg_weapons_editor_weapons", ImVec2{250.f, 0} * App::DPIHandler::get_ui_scale(), 0, ImGuiWindowFlags_HorizontalScrollbar);
    {
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Weapons");
        ImGui::BeginChild("##orpg_weapons_editor_weapons_list", ImVec2{0, ImGui::GetContentRegionMax().y - App::DPIHandler::scale_value(108)});
        {
          ImGui::BeginGroup();
          {
            for (auto& weapon : m_weapons.weapons()) {
              if (weapon.id() == 0) {
                continue;
              }

              if (ImGui::Selectable(Database::instance()->weaponNameAndId(weapon.id()).c_str(), &weapon == m_selectedWeapon) || (ImGui::IsItemFocused() && m_selectedWeapon != &weapon)) {
                m_selectedWeapon = &weapon;
                m_traitsEditor.setTraits(&m_selectedWeapon->traits());
              }
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        char str[4096];
        snprintf(str, 4096, "Max Weapons %i", m_maxWeapons);
        ImGui::SeparatorText(str);
        if (ImGui::Button("Change Max", ImVec2{ImGui::GetContentRegionMax().x - App::DPIHandler::scale_value(8), 0})) {
          m_changeIntDialogOpen = true;
          m_editMaxWeapons = m_maxWeapons;
        }
      }
      ImGui::EndGroup();
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_weapons_editor_weapons_weapon_properties");
    {
      if (m_selectedWeapon) {
        ImGui::BeginChild("##orpg_weapons_weapon_panel_left", ImVec2{ImGui::GetContentRegionMax().x / 2, 0.f});
        {
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("General Settings");
            // Name
            ImGui::BeginGroup();
            {
              char name[4096];
              strncpy(name, m_selectedWeapon->name().c_str(), 4096);
              if (ImGui::LabelOverLineEdit("##orpg_weapons_editor_name", "Name:", name, 4096, ImGui::GetContentRegionMax().x / 2 - App::DPIHandler::scale_value(16))) {
                m_selectedWeapon->setName(name);
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            // Icon
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::TextUnformatted("Icon:");
              if (ImGui::ImageButtonEx(ImGui::GetID("##orpg_database_weapons_image"), m_itemButtonTexture->get(), ImVec2(m_itemButtonTexture->size()) * App::DPIHandler::get_ui_scale(), {0.f, 0.f},
                                       {1.f, 1.f}, {}, {1.f, 1.f, 1.f, 1.f}, ImGuiButtonFlags_PressedOnDoubleClick)) {
                m_currentSheet = &m_itemSheet.value();
                // m_characterPicker->setCharacterInfo("", m_selectedItem->iconIndex);
                // m_characterPicker->setOpen(true);
              }
            }
            ImGui::EndGroup();
            // Description
            ImGui::BeginGroup();
            {
              char description[4096];
              strncpy(description, m_selectedWeapon->description().c_str(), 4096);
              ImGui::Text("Description:");
              ImGui::InputTextMultiline("##orpg_database_weapons_description", description, 4096, ImVec2{App::DPIHandler::scale_value(360), App::DPIHandler::scale_value(60)});
            }
            ImGui::EndGroup();
            // Weapon Type
            ImGui::BeginGroup();
            {
              ImGui::Text("Weapon Type:");
              // TODO: Needs to be weapon type
              // ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              // if (ImGui::BeginCombo("##orpg_database_items_itype", DecodeEnumName(magic_enum::enum_name(static_cast<ItemType>(m_selectedItem->itypeId))).c_str())) {
              //  int index{0};
              //  for (auto v : magic_enum::enum_values<ItemType>()) {
              //    if (ImGui::Selectable(DecodeEnumName(v).c_str(), static_cast<ItemType>(m_selectedItem->itypeId) == v)) {
              //      m_selectedItem->itypeId = static_cast<int>(v);
              //    }
              //  }
              //  ImGui::EndCombo();
              //}
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            // Price
            ImGui::BeginGroup();
            {
              ImGui::Text("Price:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              int price = m_selectedWeapon->price();
              if (ImGui::InputInt("##orpg_database_weapons_price", &price, 1, 100)) {
                m_selectedWeapon->setPrice(std::clamp(price, 0, 999999));
              }
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Text("Animation:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(160));
              // Animation Button
              ImGui::PushID("##orpg_database_weapon_animation");
              if (ImGui::Button(m_selectedWeapon->animationId() == -1  ? "Normal Attack"
                                : m_selectedWeapon->animationId() == 0 ? "None"
                                                                       : Database::instance()->animationName(m_selectedWeapon->animationId()).c_str(),
                                ImVec2{200 - 15 * App::DPIHandler::get_ui_scale(), 0})) {
                m_animationPicker = ObjectPicker("Animation"sv, Database::instance()->animations.animations(), m_selectedWeapon->animationId());
                m_animationPicker->setOpen(true);
              }
              ImGui::PopID();
            }
            ImGui::EndGroup();
          }
          ImGui::EndGroup();
          ImGui::BeginGroup();
          {
            ImGui::EndGroup();
            ImGui::SeparatorText("Parameter Changes");
            ImGui::BeginGroup();
            {
              ImGui::Text("Attack:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              int param = m_selectedWeapon->param(0);
              if (ImGui::InputInt("##orpg_database_weapon_attack", &param, 1, 100)) {
                m_selectedWeapon->setParam(0, std::clamp(param, -999, 999));
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("Defense:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              int param = m_selectedWeapon->param(1);
              if (ImGui::InputInt("##orpg_database_weapon_defense", &param, 1, 100)) {
                m_selectedWeapon->setParam(1, std::clamp(param, -999, 999));
              }
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Text("M.Attack:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              int param = m_selectedWeapon->param(2);
              if (ImGui::InputInt("##orpg_database_weapon_mattack", &param, 1, 100)) {
                m_selectedWeapon->setParam(2, std::clamp(param, -999, 999));
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("M.Defense:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              int param = m_selectedWeapon->param(3);
              if (ImGui::InputInt("##orpg_database_weapon_mdefense", &param, 1, 100)) {
                m_selectedWeapon->setParam(3, std::clamp(param, -999, 999));
              }
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Text("Agility:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              int param = m_selectedWeapon->param(4);
              if (ImGui::InputInt("##orpg_database_weapon_agility", &param, 1, 100)) {
                m_selectedWeapon->setParam(4, std::clamp(param, -999, 999));
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("Luck:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              int param = m_selectedWeapon->param(5);
              if (ImGui::InputInt("##orpg_database_weapon_luck", &param, 1, 100)) {
                m_selectedWeapon->setParam(5, std::clamp(param, -999, 999));
              }
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Text("Max HP:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              int param = m_selectedWeapon->param(6);
              if (ImGui::InputInt("##orpg_database_weapon_maxhp", &param, 1, 100)) {
                m_selectedWeapon->setParam(6, std::clamp(param, -9999, 9999));
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("Max MP:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              int param = m_selectedWeapon->param(7);
              if (ImGui::InputInt("##orpg_database_weapon_maxmp", &param, 1, 100)) {
                m_selectedWeapon->setParam(7, std::clamp(param, -9999, 9999));
              }
            }
            ImGui::EndGroup();
          }
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
            strncpy(note, m_selectedWeapon->note().c_str(), IM_ARRAYSIZE(note));
            if (ImGui::InputTextMultiline("##orpg_database_weapons_note", note, IM_ARRAYSIZE(note),
                                          ImVec2{ImGui::GetContentRegionMax().x - App::DPIHandler::scale_value(16), ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(16)})) {
              m_selectedWeapon->setNote(note);
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
      if (ImGui::Begin("Change Max Weapons", &m_changeIntDialogOpen,
                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
        ImGui::Text(
            "Specify an amount to resize the weapon list to\n"
            "This cannot be undone!");
        ImGui::InputInt("##value_input", &m_editMaxWeapons);
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
            const int tmpId = m_selectedWeapon->id();
            m_maxWeapons = m_editMaxWeapons;
            m_weapons.resize(m_maxWeapons);
            m_selectedWeapon = m_weapons.weapon(tmpId);
            m_changeIntDialogOpen = false;
            m_changeConfirmDialogOpen = false;
          }
          ImGui::SameLine();
          if (ImGui::Button("Cancel")) {
            m_editMaxWeapons = m_maxWeapons;
            m_changeIntDialogOpen = false;
            m_changeConfirmDialogOpen = false;
          }
        }
        ImGui::End();
      }
    }
  }
}