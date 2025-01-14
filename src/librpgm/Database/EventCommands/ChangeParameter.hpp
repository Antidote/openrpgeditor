#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeParameterCommand final : IEventCommand {
  ChangeParameterCommand() = default;
  explicit ChangeParameterCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeParameterCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Parameter; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeParameterCommand>(*this); }

  ActorComparisonSource comparison = ActorComparisonSource::Fixed;
  int value{1};
  ParameterSource param = ParameterSource::Max_HP;
  QuantityChangeOp quantityOp = QuantityChangeOp::_plu__del_Increase;
  QuantityChangeSource quantitySource = QuantityChangeSource::Constant;
  int quantity{1};
};