#include "Dialog_EnemyRecoverAll.hpp"

#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Core/Project.hpp"
#include "Database/Database.hpp"

std::tuple<bool, bool> Dialog_EnemyRecoverAll::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{181, 94} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize)) {

    ImGui::SeparatorText("Enemy");
    ImGui::SetNextItemWidth(100 * App::DPIHandler::get_ui_scale());

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

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->troop = m_troop_selection - 1;
      ImGui::CloseCurrentPopup();
      SetOpen(false);
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
      SetOpen(false);
    }
    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}