#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/TextEditor.hpp"
#include "Database/EventCommands/ChangeVehicleImage.hpp"
#include "Core/CharacterSheet.hpp"
#include "Core/CommonUI/CharacterPicker.hpp"

struct Project;
struct Dialog_ChangeVehicleImage : IEventDialogController {
  Dialog_ChangeVehicleImage() = delete;
  explicit Dialog_ChangeVehicleImage(const std::string& name, Project* project) : IEventDialogController(name), m_project(project) {
    command.reset(new ChangeVehicleImageCommand());
    m_image = command->picture;
    m_character = command->pictureIndex;
    m_vehicle = static_cast<int>(command->vehicle);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  std::string m_image;
  int m_character;
  int m_vehicle;

  std::optional<CharacterSheet> m_characterSheet;
  CheckerboardTexture m_buttonBack{80, 102, CellSizes::_64, 255, 200};
  CharacterPicker m_characterPicker{CharacterPicker::PickerMode::Character};

  bool m_confirmed{false};
  std::shared_ptr<ChangeVehicleImageCommand> command;
  std::tuple<bool, bool> result;
};