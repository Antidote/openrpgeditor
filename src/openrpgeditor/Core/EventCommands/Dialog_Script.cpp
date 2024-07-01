#include "Dialog_Script.hpp"
#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Core/Log.hpp"

std::tuple<bool, bool> Dialog_Script::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
    // SetOpen(false);
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{600, 600} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar)) {

    static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
    if (ImGui::InputTextMultiline("##source", script, IM_ARRAYSIZE(script), ImVec2(ImGui::GetContentRegionMax().x - 50, ImGui::GetContentRegionMax().y - 50), flags))
    {

    }
    if (ImGui::Button("OK")) {
      std::vector<std::string> scripts = splitString(script, '\n');
      if (scripts.size() > 1)
        m_isNext = true;

      command->script = scripts.front();
      APP_DEBUG(command->script);
      if (m_isNext) {
        command->moreScript.clear();
        command->moreScript.reserve(scripts.size());
        for (auto str = std::next(scripts.begin()); str != scripts.end(); ++str) {
          command->moreScript.emplace_back(std::make_shared<NextScriptCommand>())->script = *str;
          APP_DEBUG(command->moreScript.back()->script);
        }
        command->moreScript.shrink_to_fit();
      }
      ImGui::CloseCurrentPopup();
      SetOpen(false);
      m_confirmed = true;
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
