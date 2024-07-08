#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/Actors.hpp"
#include "Database/EventCommands/ChangeName.hpp"

struct Project;
struct Dialog_ChangeName : IEventDialogController {
  Dialog_ChangeName() = delete;
  explicit Dialog_ChangeName(const std::string& name, Project* project) : IEventDialogController(name), m_project(project) {
    command.reset(new ChangeNameCommand());
    m_actor = command->actor;
    m_str = command->name;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int m_actor;
  std::string m_str;

  bool m_confirmed{false};
  std::optional<ObjectPicker<Actor>> actor_picker;
  std::shared_ptr<ChangeNameCommand> command;
  std::tuple<bool, bool> result;
};