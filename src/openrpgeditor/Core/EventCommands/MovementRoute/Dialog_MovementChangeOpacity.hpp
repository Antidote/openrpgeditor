#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/TextEditor.hpp"
#include "Database/EventCommands/MovementRoute/Wait.hpp"
#include "Database/EventCommands/MovementRoute/ChangeOpacity.hpp"

struct Project;
struct Dialog_MovementChangeOpacity : IEventDialogController {
  Dialog_MovementChangeOpacity() = delete;
  explicit Dialog_MovementChangeOpacity(const std::string& name, Project* project) : IEventDialogController(name), m_project(project) {
    command.reset(new MovementChangeOpacityCommand());
    m_opacity = command->opacity;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int m_opacity;
  bool m_confirmed{false};
  std::shared_ptr<MovementChangeOpacityCommand> command;
  std::tuple<bool, bool> result;
};