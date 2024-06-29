#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementMoveAwayFromPlayerCommand : IEventCommand {
  ~MovementMoveAwayFromPlayerCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_away_from_Player; }
};