#include "Dialog_ControlSelfSwitch.hpp"

#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Core/Project.hpp"
#include "Database/Database.hpp"

std::tuple<bool, bool> Dialog_ControlSelfSwitch::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{168, 140} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize)) {

    ImGui::SeparatorText("Self Switch");

    ImGui::SetNextItemWidth(148 * App::DPIHandler::get_ui_scale());
    if (ImGui::BeginCombo("##selfswitch_list", m_selfSw.c_str())) {
      for (const auto self : {"A", "B", "C", "D"}) {
        if (ImGui::Selectable(self, self == m_selfSw)) {
          m_selfSw = self;
        }
      }
      ImGui::EndCombo();
    }
    ImGui::SeparatorText("Operation");
    ImGui::RadioButton("ON", &m_turnOff, 0);
    ImGui::SameLine();
    ImGui::RadioButton("OFF", &m_turnOff, 1);

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->selfSw = m_selfSw;
      command->turnOff = static_cast<ValueControl>(m_turnOff);
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