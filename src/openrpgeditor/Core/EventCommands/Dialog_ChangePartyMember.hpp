#pragma once
#include "IDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/Actors.hpp"
#include "Database/States.hpp"
#include "Database/EventCommands/ChangePartyMember.hpp"

struct Project;
struct Dialog_ChangePartyMember : IDialogController {
  Dialog_ChangePartyMember() = delete;
  explicit Dialog_ChangePartyMember(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
command = new ChangePartyMemberCommand();
    m_actor = command->member;
    m_operation = static_cast<int>(command->operation);
    m_isInit = command->initialize;
  }
  std::tuple<bool, bool> draw() override;

  IEventCommand* getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int m_actor;
  int m_operation;
  bool m_isInit;

  bool m_confirmed{false};
  std::optional<ObjectPicker<Actor>> actor_picker;
ChangePartyMemberCommand* command;
  std::tuple<bool, bool> result;
};