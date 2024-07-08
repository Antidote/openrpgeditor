#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ChangeEncounterDisable.hpp"

struct Project;
struct Dialog_ChangeEncounter : IEventDialogController {
  Dialog_ChangeEncounter() = delete;
  explicit Dialog_ChangeEncounter(const std::string& name, Project* project)
  : IEventDialogController(name), m_project(project) {
    command.reset(new ChangeEncounterDisableCommand());
    isDisabled = static_cast<int>(command->access);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int isDisabled = 0;
  bool m_confirmed{false};
  std::shared_ptr<ChangeEncounterDisableCommand> command;
  std::tuple<bool, bool> result;
};