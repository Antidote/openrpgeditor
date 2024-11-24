#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ChangeMapDisplayName.hpp"

struct Dialog_ChangeMapDisplayName : IEventDialogController {
  Dialog_ChangeMapDisplayName() = delete;
  explicit Dialog_ChangeMapDisplayName(const std::string& name, const std::shared_ptr<ChangeMapNameDisplayCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ChangeMapNameDisplayCommand());
    }
    m_checkIfOn = static_cast<int>(command->checkIfOn);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_checkIfOn;
  bool m_confirmed{false};
  std::shared_ptr<ChangeMapNameDisplayCommand> command;
  std::tuple<bool, bool> result;
};