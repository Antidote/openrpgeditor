#include "Core/EventCommands/Dialog_ChangeBattleBack.hpp"

#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_ChangeBattleBack::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{248, 100}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    if (const auto [closed, confirmed] = m_imagePicker->draw(); closed) {
      if (confirmed) {
        m_imagePicker->accept();
        m_image = m_imagePicker->selectedImage();
        m_image_2 = m_imagePicker->selectedImage2();
      }
    }
    ImGui::Text("Battle Background:");
    ImGui::PushID("#battleback_image_selection");
    if (ImGui::Button(m_image_2.empty() ? m_image.c_str() : (m_image + " & " + m_image_2).c_str(), ImVec2{300, 0})) {
      m_imagePicker->setOpen(true);
    }
    ImGui::PopID();

    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->battleBack1Name = m_image;
      command->battleBack2Name = m_image_2;
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
