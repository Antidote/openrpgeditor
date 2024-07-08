#include "Dialog_ChangeNickname.hpp"

#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Core/Project.hpp"
#include "Database/Database.hpp"

std::tuple<bool, bool> Dialog_ChangeNickname::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{183, 135} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize)) {

      if (actor_picker) {
        auto [closed, confirmed] = actor_picker->draw();
        if (confirmed) {
          m_actor = actor_picker->selection();
          actor_picker.reset();
        }
      }

    ImGui::SeparatorText("Actor");

    // Actor Button
    ImGui::PushID("##nickname_selection_actor");
    if (ImGui::Button(
            Database::Instance->actorName(m_actor).c_str(),
            ImVec2{(App::DPIHandler::scale_value(160)), 0})) {
      actor_picker = ObjectPicker<Actor>("Actor"sv, Database::Instance->actors.actorList(), 0);
            }
    ImGui::PopID();

    ImGui::SeparatorText("Nickname");
    ImGui::SetNextItemWidth(App::DPIHandler::scale_value(160));
    ImGui::InputText("##nickname_input", &m_nickname);


    if (ImGui::Button("OK")) {
      m_confirmed = true;
      command->actor = m_actor;
      command->nick = m_nickname;
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