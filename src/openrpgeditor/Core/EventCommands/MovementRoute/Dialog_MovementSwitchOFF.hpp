#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/MovementRoute/SwitchOFF.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Database/Database.hpp"

struct Project;
struct Dialog_MovementSwitchOFF : IEventDialogController {
  Dialog_MovementSwitchOFF() = delete;
  explicit Dialog_MovementSwitchOFF(const std::string& name,
                                    const std::shared_ptr<MovementSwitchOFFCommand>& cmd = nullptr)
  : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new MovementSwitchOFFCommand());
    }
    picker.emplace("Switches", Database::Instance->system.switches);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

  Project* m_project = nullptr;

private:
  bool m_confirmed{false};
  std::shared_ptr<MovementSwitchOFFCommand> command;
  std::optional<VariableSwitchPicker> picker;
  std::tuple<bool, bool> result;
};