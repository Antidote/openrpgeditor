#include "Dialog_MovementChangeFrequency.hpp"
#include <tuple>
#include "imgui.h"
#include "Core/Application.hpp"
#include "Core/DPIHandler.hpp"

std::tuple<bool, bool> Dialog_MovementChangeFrequency::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{140, 97} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize)) {

    ImGui::SeparatorText("Frequency");
    ImGui::PushItemWidth((App::DPIHandler::scale_value(100)));
    if (ImGui::BeginCombo("##movement_frequency_selection", DecodeEnumName(static_cast<MovementFrequency>(m_frequency)).c_str())) {
      for (auto& freq : magic_enum::enum_values<MovementFrequency>()) {
        bool is_selected = m_frequency == static_cast<int>(freq);
        if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(freq)).c_str(), is_selected)) {
          m_frequency = static_cast<int>(freq);
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }


    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->frequency = m_frequency;
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