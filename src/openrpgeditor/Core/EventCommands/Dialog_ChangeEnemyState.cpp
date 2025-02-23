#include "Core/EventCommands/Dialog_ChangeEnemyState.hpp"

#include "Database/Database.hpp"
#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_ChangeEnemyState::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{248, 182}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    if (state_picker) {
      auto [closed, confirmed] = state_picker->draw();
      if (closed) {
        if (confirmed) {
          m_state = state_picker->selection();
        }
        state_picker.reset();
      }
    }

    ImGui::SeparatorText("Enemy");

    ImGui::SetNextItemWidth(160);

    if (ImGui::BeginCombo("##enemystate_change_list", m_troop_selection == 0 ? "Entire Troop" : ("#" + std::to_string(m_troop_selection) + " ?").c_str())) {
      if (ImGui::Selectable("Entire Troop", m_troop_selection == 0)) {
        m_troop_selection = 0;
      }
      for (int i = 1; i < 9; ++i) {
        std::string text = "#" + std::to_string(i) + " ?";
        if (ImGui::Selectable(text.c_str(), i == m_troop_selection)) {
          m_troop_selection = i;
        }
      }
      ImGui::EndCombo();
    }

    ImGui::SeparatorText("Operation");
    ImGui::RadioButton("Increase", &m_operation, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Decrease", &m_operation, 1);

    ImGui::SeparatorText("State");
    ImGui::SetNextItemWidth(100);
    std::string text = Database::instance()->stateName(m_state);
    ImGui::PushID("##enemystate_change");
    if (ImGui::Button(text.c_str(), ImVec2{200 - 15, 0})) {
      state_picker = ObjectPicker<State>("State"sv, Database::instance()->states.states(), m_state);
      state_picker->setOpen(true);
    }
    ImGui::PopID();

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->enemy = m_troop_selection - 1;
      command->enemyOp = static_cast<PartyMemberOperation>(m_operation);
      command->state = m_state;
      ImGui::CloseCurrentPopup();
      setOpen(false);
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
      setOpen(false);
    }
    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
