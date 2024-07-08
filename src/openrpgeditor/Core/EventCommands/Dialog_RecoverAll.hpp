#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Database/Actors.hpp"
#include "Database/EventCommands/ChangeGold.hpp"
#include "Database/EventCommands/RecoverAll.hpp"

struct Project;
struct Dialog_RecoverAll : IEventDialogController {
  Dialog_RecoverAll() = delete;
  explicit Dialog_RecoverAll(const std::string& name, Project* project) : IEventDialogController(name), m_project(project) {
    command.reset(new RecoverAllCommand());
    m_actor = command->value;
    m_variable = command->value;
    m_Source = static_cast<int>(command->comparison);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int m_actor;
  int m_variable;
  int m_Source;

  bool m_confirmed{false};
  std::optional<ObjectPicker<Actor>> actor_picker;
  std::optional<VariableSwitchPicker> picker;
  std::shared_ptr<RecoverAllCommand> command;
  std::tuple<bool, bool> result;
};