#include "Core/CommonUI/EventCommandEditor.hpp"
#include "Core/Project.hpp"
#include "Core/DPIHandler.hpp"
#include "Core/Application.hpp"
#include "Core/ImGuiParsedText.hpp"
#include "Core/ImGuiUtils.hpp"

#include "imgui.h"
#include "imgui_internal.h"
#include "Core/EventCommands/Dialog_ControlSwitches.hpp"
#include "Core/EventCommands/Dialog_ControlVariables.hpp"
#include "Core/EventCommands/IDialogController.hpp"

#include "Database/EventCommands/BattleProcessing.hpp"
#include "Database/EventCommands/ChangeActorImage.hpp"
#include "Database/EventCommands/ChangeArmors.hpp"
#include "Database/EventCommands/ChangeClass.hpp"
#include "Database/EventCommands/ChangeEXP.hpp"
#include "Database/EventCommands/ChangeEnemyHP.hpp"
#include "Database/EventCommands/ChangeEnemyMP.hpp"
#include "Database/EventCommands/ChangeEnemyState.hpp"
#include "Database/EventCommands/ChangeEnemyTP.hpp"
#include "Database/EventCommands/ChangeEquipment.hpp"
#include "Database/EventCommands/ChangeHP.hpp"
#include "Database/EventCommands/ChangeItems.hpp"
#include "Database/EventCommands/ChangeLevel.hpp"
#include "Database/EventCommands/ChangeMP.hpp"
#include "Database/EventCommands/ChangeName.hpp"
#include "Database/EventCommands/ChangeNickname.hpp"
#include "Database/EventCommands/ChangeParameter.hpp"
#include "Database/EventCommands/ChangePartyMember.hpp"
#include "Database/EventCommands/ChangeProfile.hpp"
#include "Database/EventCommands/ChangeSkill.hpp"
#include "Database/EventCommands/ChangeState.hpp"
#include "Database/EventCommands/ChangeTP.hpp"
#include "Database/EventCommands/ChangeTileset.hpp"
#include "Database/EventCommands/ChangeWeapons.hpp"
#include "Database/EventCommands/CommonEvent.hpp"
#include "Database/EventCommands/ConditionalBranch.hpp"
#include "Database/EventCommands/EnemyAppear.hpp"
#include "Database/EventCommands/EnemyRecoverAll.hpp"
#include "Database/EventCommands/EnemyTransform.hpp"
#include "Database/EventCommands/ForceAction.hpp"
#include "Database/EventCommands/GetLocationInfo.hpp"
#include "Database/EventCommands/InputNumber.hpp"
#include "Database/EventCommands/NameInput.hpp"
#include "Database/EventCommands/RecoverAll.hpp"
#include "Database/EventCommands/SelectItem.hpp"
#include "Database/EventCommands/SetEventLocation.hpp"
#include "Database/EventCommands/SetMovementRoute.hpp"
#include "Database/EventCommands/SetVehicleLocation.hpp"
#include "Database/EventCommands/ShopProcessing.hpp"
#include "Database/EventCommands/ShowAnimation.hpp"
#include "Database/EventCommands/ShowBalloonIcon.hpp"
#include "Database/EventCommands/ShowBattleAnimation.hpp"
#include "Database/EventCommands/TransferPlayer.hpp"

void insertValue(std::string& indentPad, const std::string& val, const std::string& delim) {
  auto pos = indentPad.find(delim);
  if (pos != std::string::npos) {
    indentPad.replace(pos, 2, val);
  }
}
void EventCommandEditor::draw() {
  ImGui::BeginGroup();
  {
    ImGui::Text("Content:");

    if (ImGui::BeginTable("##commonevent_code_contents", 2,
                          ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX |
                              ImGuiTableFlags_ScrollY,
                          ImVec2{0, ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(16)})) {

      ImGui::TableSetupColumn("Selectable", ImGuiTableFlags_SizingFixedFit);
      ImGui::TableSetupScrollFreeze(1, 0);
      ImGui::TableSetupColumn("Text");
      const int totalPadding = static_cast<int>(std::floor(std::log10(m_commands->size())));

      if (m_commands) {
        ImGui::PushFont(App::APP->getMonoFont());
        for (int n = 0; n < m_commands->size(); n++) {
          const bool isSelected = (m_selectedCommand == n);
          std::string indentPad = m_commands->at(n)->stringRep(m_project->database());
          if (m_commands->at(n)->code() == EventCode::Common_Event) {
            // Common Event + (name)
            CommonEventCommand* cec = dynamic_cast<CommonEventCommand*>(m_commands->at(n).get());
            indentPad += m_project->commonEvent(cec->event)->name.c_str();
          } else if (m_commands->at(n)->code() == EventCode::Conditional_Branch) {
            ConditionalBranchCommand* cb = dynamic_cast<ConditionalBranchCommand*>(m_commands->at(n).get());

            if (cb->type == ConditionType::Variable) {
              // Constant and variable sources
              insertValue(indentPad, m_project->variable(cb->variable.id).c_str(), "{");
              if (cb->variable.source == VariableComparisonSource::Variable) {
                insertValue(indentPad, m_project->variable(cb->variable.otherId).c_str(), "{");
              }
            } else if (cb->type == ConditionType::Switch) {
              insertValue(indentPad, m_project->switche(cb->globalSwitch.switchIdx).c_str(), "{");
            } else if (cb->type == ConditionType::Self_Switch) {
              insertValue(indentPad, cb->selfSw, "{");
            } else if (cb->type == ConditionType::Actor) {
              insertValue(indentPad, m_project->actor(cb->actor.id)->name, "{");
              if (cb->actor.type != ActorConditionType::In_The_Party) {
                ActorConditionType actorCondition = cb->actor.type;
                if (actorCondition == ActorConditionType::Class) {
                  insertValue(indentPad, m_project->actorClass(cb->actor.checkId)->name, "[");
                } else if (actorCondition == ActorConditionType::Skill) {
                  insertValue(indentPad, m_project->skill(cb->actor.checkId)->name, "[");
                } else if (actorCondition == ActorConditionType::Weapon) {
                  insertValue(indentPad, m_project->weapon(cb->actor.checkId)->name, "[");
                } else if (actorCondition == ActorConditionType::Armor) {
                  insertValue(indentPad, m_project->armor(cb->actor.checkId)->name, "{");
                } else if (actorCondition == ActorConditionType::State) {
                  insertValue(indentPad, m_project->state(cb->actor.checkId)->name, "{");
                } else {
                  insertValue(indentPad, cb->name, "{");
                }
              }
            } else if (cb->type == ConditionType::Enemy) {
              insertValue(indentPad, m_project->enemy(cb->enemy.id)->name, "{");
              if (cb->enemy.type != EnemyConditionType::Appeared) {
                insertValue(indentPad, m_project->state(cb->enemy.stateId)->name, "{");
              }
            } else if (cb->type == ConditionType::Character) {
              std::string characterName = "Error!!";
              if (cb->character.id < 0) {
                characterName = cb->character.id == -1 ? "Player" : "This Event";
              } else {
                characterName = m_project->event(cb->character.id)->name;
              }
              insertValue(indentPad, characterName, "{");
            } else if (cb->type == ConditionType::Vehicle) {
              insertValue(indentPad, m_project->vehicle(cb->vehicle.id), "{");
            } else if (cb->type == ConditionType::Gold) {
              insertValue(indentPad, std::to_string(cb->gold.value), "{");
            } else if (cb->type == ConditionType::Item) {
              insertValue(indentPad, m_project->item(cb->item.id)->name, "{");
            } else if (cb->type == ConditionType::Weapon) {
              insertValue(indentPad, m_project->weapon(cb->item.id)->name, "{");
            } else if (cb->type == ConditionType::Armor) {
              insertValue(indentPad, m_project->armor(cb->item.id)->name, "{");
            } else if (cb->type == ConditionType::Button) {
              insertValue(indentPad, cb->button, "{");
            } else if (cb->type == ConditionType::Script) {
              indentPad += cb->script;
            }
          } else if (m_commands->at(n)->code() == EventCode::Control_Switches) {
            ControlSwitches* cs = dynamic_cast<ControlSwitches*>(m_commands->at(n).get());
            if (cs->start == cs->end) {
              insertValue(indentPad, m_project->switche(cs->start), "{");
            }
          } else if (m_commands->at(n)->code() == EventCode::Control_Variables) {
            ControlVariables* cv = dynamic_cast<ControlVariables*>(m_commands->at(n).get());

            if (cv->start == cv->end) {
              insertValue(indentPad, m_project->variable(cv->start), "{");
              if (cv->operand == VariableControlOperand::Variable) {
                insertValue(indentPad, m_project->variable(cv->variable), "{");
              } else if (cv->operand == VariableControlOperand::Game_Data) {
                if (cv->gameData.source == GameDataSource::Actor) {
                  insertValue(indentPad, m_project->actor(cv->gameData.rawSource)->name, "{");
                } else if (cv->gameData.source == GameDataSource::Armor) {
                  insertValue(indentPad, m_project->armor(cv->gameData.rawSource)->name, "{");

                } else if (cv->gameData.source == GameDataSource::Character) {
                  insertValue(indentPad, m_project->event(cv->gameData.rawSource)->name, "{");

                } else if (cv->gameData.source == GameDataSource::Enemy) {
                  insertValue(indentPad, m_project->enemy(cv->gameData.rawSource)->name, "{");

                } else if (cv->gameData.source == GameDataSource::Item) {
                  insertValue(indentPad, m_project->item(cv->gameData.rawSource)->name, "{");

                } else if (cv->gameData.source == GameDataSource::Weapon) {
                  insertValue(indentPad, m_project->weapon(cv->gameData.rawSource)->name, "{");
                }
              }
            }
          } else if (m_commands->at(n)->code() == EventCode::Change_Armors) {
            ChangeArmorsCommmand* ca = dynamic_cast<ChangeArmorsCommmand*>(m_commands->at(n).get());
            insertValue(indentPad, m_project->armor(ca->item)->name, "{");
            if (ca->operandSource == QuantityChangeSource::Variable)
              insertValue(indentPad, m_project->armor(ca->item)->name, "[");
          } else if (m_commands->at(n)->code() == EventCode::Change_Weapons) {
            ChangeWeaponsCommmand* cw = dynamic_cast<ChangeWeaponsCommmand*>(m_commands->at(n).get());
            insertValue(indentPad, m_project->weapon(cw->item)->name, "{");
            if (cw->operandSource == QuantityChangeSource::Variable)
              insertValue(indentPad, m_project->weapon(cw->item)->name, "[");
          } else if (m_commands->at(n)->code() == EventCode::Change_Items) {
            ChangeItemsCommmand* ci = dynamic_cast<ChangeItemsCommmand*>(m_commands->at(n).get());
            insertValue(indentPad, m_project->item(ci->item)->name, "{");
            if (ci->operandSource == QuantityChangeSource::Variable)
              insertValue(indentPad, m_project->item(ci->item)->name, "[");
          } else if (m_commands->at(n)->code() == EventCode::Change_Party_Member) {
            ChangePartyMemberCommand* cm = dynamic_cast<ChangePartyMemberCommand*>(m_commands->at(n).get());
            insertValue(indentPad, m_project->actor(cm->member)->name, "{");
          } else if (m_commands->at(n)->code() == EventCode::Show_Animation) {
            ShowAnimationCommand* sac = dynamic_cast<ShowAnimationCommand*>(m_commands->at(n).get());
            insertValue(indentPad, m_project->animation(sac->animation)->name, "{");
          } else if (m_commands->at(n)->code() == EventCode::Set_Movement_Route) {
            if (indentPad.find("{") > 0) {
              SetMovementRouteCommand* smr = dynamic_cast<SetMovementRouteCommand*>(m_commands->at(n).get());
              insertValue(indentPad, m_project->event(smr->character)->name, "{");
            }
          } else if (m_commands->at(n)->code() == EventCode::Transfer_Player) {
            TransferPlayerCommand* tpc = dynamic_cast<TransferPlayerCommand*>(m_commands->at(n).get());
            if (tpc->mode == TransferMode::Variable_Designation) {
              insertValue(indentPad, m_project->map(tpc->mapId)->name, "[");
              insertValue(indentPad, m_project->variable(tpc->x), "[");
              insertValue(indentPad, m_project->variable(tpc->y), "[");
            } else {
              insertValue(indentPad, std::format("{:04}", tpc->mapId) + m_project->map(tpc->mapId)->name, "{");
            }
          } else if (m_commands->at(n)->code() == EventCode::Set_Event_Location) {
            SetEventLocationCommand* sel = dynamic_cast<SetEventLocationCommand*>(m_commands->at(n).get());

            if (sel->event > 0)
              insertValue(indentPad, m_project->event(sel->event)->name, "{");

            if (sel->mode == TransferMode::Variable_Designation) {
              insertValue(indentPad, m_project->variable(sel->x), "[");
              insertValue(indentPad, m_project->variable(sel->y), "[");
            } else if (sel->mode == TransferMode::Exchange_With_Another_Event) {
              if (sel->x > 0)
                insertValue(indentPad, m_project->event(sel->event)->name, "<");
            }
          } else if (m_commands->at(n)->code() == EventCode::Set_Vehicle_Location) {
            SetVehicleLocationCommand* svl = dynamic_cast<SetVehicleLocationCommand*>(m_commands->at(n).get());

            if (svl->mode == TransferMode::Variable_Designation) {
              insertValue(indentPad, m_project->map(svl->mapId)->name, "[");
              insertValue(indentPad, m_project->variable(svl->x), "[");
              insertValue(indentPad, m_project->variable(svl->y), "[");
            } else {
              insertValue(indentPad, std::format("{:04}", svl->mapId) + m_project->map(svl->mapId)->name, "{");
            }
          } else if (m_commands->at(n)->code() == EventCode::Select_Item) {
            SelectItemCommand* sic = dynamic_cast<SelectItemCommand*>(m_commands->at(n).get());
            insertValue(indentPad, m_project->item(sic->item)->name, "{");
          } else if (m_commands->at(n)->code() == EventCode::Input_Number) {
            InputNumberCommand* inc = dynamic_cast<InputNumberCommand*>(m_commands->at(n).get());
            insertValue(indentPad, m_project->variable(inc->variable), "{");
          } else if (m_commands->at(n)->code() == EventCode::Change_Tileset) {
            ChangeTilesetCommand* val = dynamic_cast<ChangeTilesetCommand*>(m_commands->at(n).get());
            insertValue(indentPad, m_project->tileset(val->tileset)->name, "{");
          } else if (m_commands->at(n)->code() == EventCode::Get_Location_Info) {
            GetLocationInfoCommand* val = dynamic_cast<GetLocationInfoCommand*>(m_commands->at(n).get());
            insertValue(indentPad, m_project->variable(val->variable), "{");
            if (val->source == LocationSource::Designation_with_variables) {
              insertValue(indentPad, m_project->variable(val->x), "[");
              insertValue(indentPad, m_project->variable(val->y), "[");
            }
          } else if (m_commands->at(n)->code() == EventCode::Battle_Processing) {
            BattleProcessingCommand* val = dynamic_cast<BattleProcessingCommand*>(m_commands->at(n).get());
            if (val->type == BattleProcessType::Designation_with_variables) {
              insertValue(indentPad, m_project->variable(val->id), "[");
            } else if (val->type == BattleProcessType::Direct_designation) {
              insertValue(indentPad, m_project->troop(val->id)->name, "{");
            }
          } else if (m_commands->at(n)->code() == EventCode::Shop_Processing_Good) {
            ShopProcessingGoodCommand* val = dynamic_cast<ShopProcessingGoodCommand*>(m_commands->at(n).get());
            insertValue(indentPad, m_project->item(val->id)->name, "{");
          } else if (m_commands->at(n)->code() == EventCode::Shop_Processing) {
            ShopProcessingCommand* val = dynamic_cast<ShopProcessingCommand*>(m_commands->at(n).get());
            insertValue(indentPad, m_project->item(val->id)->name, "{");
          } else if (m_commands->at(n)->code() == EventCode::Name_Input_Processing) {
            NameInputCommand* val = dynamic_cast<NameInputCommand*>(m_commands->at(n).get());
            insertValue(indentPad, m_project->actor(val->actorId)->name, "{");
          } else if (m_commands->at(n)->code() == EventCode::Change_HP) {
            ChangeHPCommand* val = dynamic_cast<ChangeHPCommand*>(m_commands->at(n).get());
            // Fixed vs Variable
            if (val->comparison == ActorComparisonSource::Variable) {
              insertValue(indentPad, "{" + m_project->variable(val->value) + "}", "{");
            } else {
              insertValue(indentPad, m_project->actor(val->value)->name, "{");
            }
            // Constant vs Variable
            if (val->quantitySource == QuantityChangeSource::Variable) {
              insertValue(indentPad, "{" + m_project->variable(val->quantity) + "}", "[");
            }
          } else if (m_commands->at(n)->code() == EventCode::Change_MP) {
            ChangeMPCommand* val = dynamic_cast<ChangeMPCommand*>(m_commands->at(n).get());
            // Fixed vs Variable
            if (val->comparison == ActorComparisonSource::Variable) {
              insertValue(indentPad, "{" + m_project->variable(val->value) + "}", "{");
            } else {
              insertValue(indentPad, m_project->actor(val->value)->name, "{");
            }
            // Constant vs Variable
            if (val->quantitySource == QuantityChangeSource::Variable) {
              insertValue(indentPad, "{" + m_project->variable(val->quantity) + "}", "[");
            }
          } else if (m_commands->at(n)->code() == EventCode::Change_TP) {
            ChangeTPCommand* val = dynamic_cast<ChangeTPCommand*>(m_commands->at(n).get());
            // Fixed vs Variable
            if (val->comparison == ActorComparisonSource::Variable) {
              insertValue(indentPad, "{" + m_project->variable(val->value) + "}", "{");
            } else {
              insertValue(indentPad, m_project->actor(val->value)->name, "{");
            }
            // Constant vs Variable
            if (val->quantitySource == QuantityChangeSource::Variable) {
              insertValue(indentPad, "{" + m_project->variable(val->quantity) + "}", "[");
            }
          } else if (m_commands->at(n)->code() == EventCode::Change_EXP) {
            ChangeEXPCommand* val = dynamic_cast<ChangeEXPCommand*>(m_commands->at(n).get());
            // Fixed vs Variable
            if (val->comparison == ActorComparisonSource::Variable) {
              insertValue(indentPad, "{" + m_project->variable(val->quantity) + "}", "{");
            } else {
              insertValue(indentPad, m_project->actor(val->value)->name, "{");
            }
            // Constant vs Variable
            if (val->quantitySource == QuantityChangeSource::Variable) {
              insertValue(indentPad, "{" + m_project->variable(val->quantity) + "}", "[");
            }
          } else if (m_commands->at(n)->code() == EventCode::Change_Level) {
            ChangeLevelCommand* val = dynamic_cast<ChangeLevelCommand*>(m_commands->at(n).get());
            // Fixed vs Variable
            if (val->comparison == ActorComparisonSource::Variable) {
              insertValue(indentPad, "{" + m_project->variable(val->value) + "}", "{");
            } else {
              insertValue(indentPad, m_project->actor(val->value)->name, "{");
            }
            // Constant vs Variable
            if (val->quantitySource == QuantityChangeSource::Variable) {
              insertValue(indentPad, "{" + m_project->variable(val->quantity) + "}", "[");
            }
          } else if (m_commands->at(n)->code() == EventCode::Change_Parameter) {
            ChangeParameterCommand* val = dynamic_cast<ChangeParameterCommand*>(m_commands->at(n).get());
            // Fixed vs Variable
            if (val->comparison == ActorComparisonSource::Variable) {
              insertValue(indentPad, "{" + m_project->variable(val->value) + "}", "{");
            } else {
              insertValue(indentPad, m_project->actor(val->value)->name, "{");
            }
            // Constant vs Variable
            if (val->quantitySource == QuantityChangeSource::Variable) {
              insertValue(indentPad, "{" + m_project->variable(val->quantity) + "}", "[");
            }
          } else if (m_commands->at(n)->code() == EventCode::Recover_All) {
            RecoverAllCommand* val = dynamic_cast<RecoverAllCommand*>(m_commands->at(n).get());
            // Fixed vs Variable
            if (val->comparison == ActorComparisonSource::Variable) {
              insertValue(indentPad, "{" + m_project->variable(val->value) + "}", "{");
            } else {
              insertValue(indentPad, m_project->actor(val->value)->name, "{");
            }
          } else if (m_commands->at(n)->code() == EventCode::Change_Name) {
            ChangeNameCommand* val = dynamic_cast<ChangeNameCommand*>(m_commands->at(n).get());
            insertValue(indentPad, m_project->actor(val->actor)->name, "{");
          } else if (m_commands->at(n)->code() == EventCode::Change_Class) {
            ChangeClassCommand* val = dynamic_cast<ChangeClassCommand*>(m_commands->at(n).get());
            insertValue(indentPad, m_project->actor(val->actor)->name, "{");
            insertValue(indentPad, m_project->actorClass(val->classId)->name, "{");
          } else if (m_commands->at(n)->code() == EventCode::Change_State) {
            ChangeStateCommand* val = dynamic_cast<ChangeStateCommand*>(m_commands->at(n).get());
            // Fixed vs Variable
            if (val->comparison == ActorComparisonSource::Variable) {
              insertValue(indentPad, "{" + m_project->variable(val->value) + "}", "{");
            } else {
              insertValue(indentPad, m_project->actor(val->value)->name, "{");
            }
            insertValue(indentPad, m_project->state(val->state)->name, "{");
          } else if (m_commands->at(n)->code() == EventCode::Change_Skill) {
            ChangeSkillCommand* val = dynamic_cast<ChangeSkillCommand*>(m_commands->at(n).get());
            // Fixed vs Variable
            if (val->comparison == ActorComparisonSource::Variable) {
              insertValue(indentPad, "{" + m_project->variable(val->value) + "}", "{");
            } else {
              insertValue(indentPad, m_project->actor(val->value)->name, "{");
            }
            insertValue(indentPad, m_project->skill(val->skill)->name, "[");
          } else if (m_commands->at(n)->code() == EventCode::Change_Equipment) {
            ChangeEquipmentCommand* val = dynamic_cast<ChangeEquipmentCommand*>(m_commands->at(n).get());
            insertValue(indentPad, m_project->actor(val->actorId)->name, "{");
            insertValue(indentPad, m_project->equipType(val->equipType), "{");
            if (val->equipment > 0) {
              // TODO: We need to obtain the equipment correctly based on equipment type
              // insertValue(indentPad, m_project->equipment(val->equipment)->name, "{");
            }
          } else if (m_commands->at(n)->code() == EventCode::Enemy_Recover_All) {
            EnemyRecoverAllCommand* val = dynamic_cast<EnemyRecoverAllCommand*>(m_commands->at(n).get());
            if (val->troop > 0) {
              insertValue(indentPad, m_project->troop(val->troop)->name, "{");
            }
          } else if (m_commands->at(n)->code() == EventCode::Enemy_Appear) {
            EnemyAppearCommand* val = dynamic_cast<EnemyAppearCommand*>(m_commands->at(n).get());
            insertValue(indentPad, m_project->troop(val->enemy)->name, "{");
          } else if (m_commands->at(n)->code() == EventCode::Enemy_Transform) {
            EnemyTransformCommand* val = dynamic_cast<EnemyTransformCommand*>(m_commands->at(n).get());
            insertValue(indentPad, m_project->troop(val->enemy)->name, "{");
            insertValue(indentPad, m_project->enemy(val->transform)->name, "{");
          } else if (m_commands->at(n)->code() == EventCode::Change_Enemy_HP) {
            ChangeEnemyHPCommand* val = dynamic_cast<ChangeEnemyHPCommand*>(m_commands->at(n).get());
            insertValue(indentPad, m_project->troop(val->enemy)->name, "{");

            // Constant vs Variable
            if (val->quantitySource == QuantityChangeSource::Variable) {
              insertValue(indentPad, "{" + m_project->variable(val->quantity) + "}", "[");
            }
          } else if (m_commands->at(n)->code() == EventCode::Change_Enemy_MP) {
            ChangeEnemyMPCommand* val = dynamic_cast<ChangeEnemyMPCommand*>(m_commands->at(n).get());
            insertValue(indentPad, m_project->troop(val->enemy)->name, "{");

            // Constant vs Variable
            if (val->quantitySource == QuantityChangeSource::Variable) {
              insertValue(indentPad, "{" + m_project->variable(val->quantity) + "}", "[");
            }
          } else if (m_commands->at(n)->code() == EventCode::Change_Enemy_TP) {
            ChangeEnemyTPCommand* val = dynamic_cast<ChangeEnemyTPCommand*>(m_commands->at(n).get());
            insertValue(indentPad, m_project->troop(val->enemy)->name, "{");

            // Constant vs Variable
            if (val->quantitySource == QuantityChangeSource::Variable) {
              insertValue(indentPad, "{" + m_project->variable(val->quantity) + "}", "[");
            }
          } else if (m_commands->at(n)->code() == EventCode::Change_Enemy_TP) {
            ChangeEnemyStateCommand* val = dynamic_cast<ChangeEnemyStateCommand*>(m_commands->at(n).get());
            if (val->enemy > 0)
              insertValue(indentPad, m_project->troop(val->enemy)->name, "{");

            insertValue(indentPad, m_project->state(val->state)->name, "{");
          } else if (m_commands->at(n)->code() == EventCode::Force_Action) {
            ForceActionCommand* val = dynamic_cast<ForceActionCommand*>(m_commands->at(n).get());
            if (val->sourceComparison == SubjectComparisonSource::Actor) {
              insertValue(indentPad, m_project->actor(val->source)->name, "{");
            } else {
              insertValue(indentPad, m_project->troop(val->source)->name, "{");
            }
            insertValue(indentPad, m_project->skill(val->skill)->name, "{");
          } else if (m_commands->at(n)->code() == EventCode::Show_Battle_Animation) {
            ShowBattleAnimationCommand* val = dynamic_cast<ShowBattleAnimationCommand*>(m_commands->at(n).get());
            if (val->enemy > 0) {
              insertValue(indentPad, m_project->troop(val->enemy)->name, "{");
            }
            insertValue(indentPad, m_project->animation(val->animation)->name, "{");
          } else if (m_commands->at(n)->code() == EventCode::Change_Actor_Images) {
            ChangeActorImageCommand* val = dynamic_cast<ChangeActorImageCommand*>(m_commands->at(n).get());
            insertValue(indentPad, m_project->actor(val->actor)->name, "{");
          } else if (m_commands->at(n)->code() == EventCode::Change_Profile) {
            ChangeProfileCommand* val = dynamic_cast<ChangeProfileCommand*>(m_commands->at(n).get());
            insertValue(indentPad, m_project->actor(val->actor)->name, "{");
          } else if (m_commands->at(n)->code() == EventCode::Change_Nickname) {
            ChangeNicknameCommand* val = dynamic_cast<ChangeNicknameCommand*>(m_commands->at(n).get());
            insertValue(indentPad, m_project->actor(val->actor)->name, "{");
          } else if (m_commands->at(n)->code() == EventCode::Show_Balloon_Icon) {
            ShowBalloonIconCommand* val = dynamic_cast<ShowBalloonIconCommand*>(m_commands->at(n).get());
            insertValue(indentPad, m_project->event(val->id)->name, "{");
          }
          ImGui::TableNextRow();
          if (ImGui::TableNextColumn()) {
            const int step = n + 1;
            const int stepPadding = (totalPadding - static_cast<int>(std::floor(std::log10(step)))) + 1;
            if (ImGui::SelectableWithBorder((std::string(stepPadding, ' ') + std::to_string(step) + " ").c_str(),
                                            isSelected,
                                            ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns |
                                                ImGuiSelectableFlags_AllowDoubleClick,
                                            ImVec2(0, ImGui::CalcTextSize(indentPad.c_str()).y))) {
              if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
                m_selectedCommand = n;
                m_isNewEntry = false;
                ImGui::OpenPopup("Command Window");
              }
              m_selectedCommand = n;
            }
          }
          if (ImGui::TableNextColumn()) {
            ImGui::TextParsed("&push-color=255,255,255;%s&pop-color;", indentPad.c_str());
          }

          if (isSelected)
            ImGui::SetItemDefaultFocus();
        }
        ImGui::PopFont();
      }
      drawPopup(m_commands->at(m_selectedCommand));

      ImGui::EndTable();
    }
  }
  ImGui::EndGroup();
}

void EventCommandEditor::drawPopup(std::shared_ptr<IEventCommand> command) {
  if (m_selectedCommand == 0) {
    return;
  }

  ImGui::SetNextWindowSize(ImVec2{680, 550} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal("Command Window", nullptr,
                             ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)) {
    if (!m_isNewEntry) {
      // We're not a new entry so copy our values so we can restore them if we cancel
      // m_tempTrait = s*m_selectedTrait;
    }
    ImGui::BeginGroup();
    {

      if (ImGui::BeginTabBar("##orpg_command_window")) {

        if (commandDialog)
          commandDialog->draw();

        ImVec2 size = ImVec2{((ImGui::GetContentRegionAvail().x / 2) / 2) - App::DPIHandler::scale_value(15), 0};

        if (ImGui::BeginTabItem("Actor")) {
          if (ImGui::Button("Change HP...", size)) {
          }
          ImGui::SameLine();
          if (ImGui::Button("Change Gold...", size)) {
          }
          ImGui::SameLine();
          if (ImGui::Button("Change Enemy HP...", size)) {
          }
          if (ImGui::Button("Change MP...", size)) {
          }
          ImGui::SameLine();
          if (ImGui::Button("Change Items...", size)) {
          }
          ImGui::SameLine();
          if (ImGui::Button("Change Enemy MP...", size)) {
          }
          if (ImGui::Button("Change TP...", size)) {
          }
          ImGui::SameLine();
          if (ImGui::Button("Change Weapons...", size)) {
          }
          ImGui::SameLine();
          if (ImGui::Button("Change Enemy TP...", size)) {
          }
          if (ImGui::Button("Change State...", size)) {
          }
          ImGui::SameLine();
          if (ImGui::Button("Change Armors...", size)) {
          }
          ImGui::SameLine();
          if (ImGui::Button("Change Enemy State...", size)) {
          }
          if (ImGui::Button("Recover All...", size)) {
          }
          ImGui::SameLine();
          if (ImGui::Button("Change Party Member...", size)) {
          }
          ImGui::SameLine();
          if (ImGui::Button("Enemy Recover All...", size)) {
          }
          if (ImGui::Button("Change EXP...", size)) {
          }
          ImGui::SameLine();
          if (ImGui::Button("Change Nickname...", size)) {
          }
          ImGui::SameLine();
          if (ImGui::Button("Enemy Appear...", size)) {
          }
          if (ImGui::Button("Change Level...", size)) {
          }
          ImGui::SameLine();
          if (ImGui::Button("Change Profile...", size)) {
          }
          ImGui::SameLine();
          if (ImGui::Button("Enemy Transform...", size)) {
          }
          if (ImGui::Button("Change Parameter...", size)) {
          }
          if (ImGui::Button("Change Skill...", size)) {
          }
          if (ImGui::Button("Change Equipment", size)) {
          }
          if (ImGui::Button("Change Name...", size)) {
          }
          if (ImGui::Button("Change Class...", size)) {
          }
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Audio")) {
          if (ImGui::Button("Play BGM...", size)) {
          }
          if (ImGui::Button("Play BGS...", size)) {
          }
          if (ImGui::Button("Play SE...", size)) {
          }
          if (ImGui::Button("Play ME...", size)) {
          }
          if (ImGui::Button("Play Movie...", size)) {
          }
          if (ImGui::Button("Save BGM", size)) {
          }
          if (ImGui::Button("Replay BGM", size)) {
          }
          if (ImGui::Button("Stop SE", size)) {
          }
          if (ImGui::Button("Fadeout BGM...", size)) {
          }
          if (ImGui::Button("Fadeout BGS...", size)) {
          }
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Flow")) {
          if (ImGui::Button("Conditional Branch...", size)) {
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Control Switches...", size)) {
            commandDialog = new Dialog_ControlSwitches("Control Switches", m_project);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Third Column
          if (ImGui::Button("Wait...", size)) {
          }
          if (ImGui::Button("Loop", size)) {
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Control Variables...", size)) {
            commandDialog = new Dialog_ControlVariables("Control Variables", m_project);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Third Column
          if (ImGui::Button("Erase Event", size)) {
          }
          if (ImGui::Button("Break Loop", size)) {
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Control Self Switch...", size)) {
          }
          ImGui::SameLine(); // Third Column
          if (ImGui::Button("Exit Event Processing", size)) {
          }
          if (ImGui::Button("Common Event...", size)) {
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Control Timer...", size)) {
          }
          if (ImGui::Button("Label...", size)) {
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Transfer Player...", size)) {
          }
          if (ImGui::Button("Jump to Label...", size)) {
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Set Event Location...", size)) {
          }
          if (ImGui::Button("Comment...", size)) {
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Set Vehicle Location...", size)) {
          }
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Scene")) {
          if (ImGui::Button("Show Text...", size)) {
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Show Animation...", size)) {
          }
          ImGui::SameLine(); // Third Column
          if (ImGui::Button("Change Transparency...", size)) {
          }
          if (ImGui::Button("Show Choices...", size)) {
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Show Battle Animation...", size)) {
          }
          ImGui::SameLine(); // Third Column
          if (ImGui::Button("Get on/off Vehicle", size)) {
          }
          if (ImGui::Button("Show Scrolling Text...", size)) {
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Show Balloon Icon...", size)) {
          }
          ImGui::SameLine(); // Third Column
          if (ImGui::Button("Abort Battle", size)) {
          }
          if (ImGui::Button("Input Number...", size)) {
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Shop Processing...", size)) {
          }
          ImGui::SameLine(); // Third Column
          if (ImGui::Button("Gather Followers", size)) {
          }
          if (ImGui::Button("Select Item...", size)) {
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Battle Processing...", size)) {
          }
          if (ImGui::Button("Set Movement Route...", size)) {
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Name Input Processing...", size)) {
          }
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Screen")) {
          if (ImGui::Button("Show Picture...", size)) {
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Tint Screen...", size)) {
          }
          ImGui::SameLine(); // Third column
          if (ImGui::Button("Set Weather Effect...", size)) {
          }
          if (ImGui::Button("Move Picture...", size)) {
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Flash Screen...", size)) {
          }
          ImGui::SameLine(); // Third column
          if (ImGui::Button("Scroll Map...", size)) {
          }
          if (ImGui::Button("Rotate Picture...", size)) {
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Shake Screen...", size)) {
          }
          if (ImGui::Button("Tint Picture...", size)) {
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Fadeout Screen...", size)) {
          }
          if (ImGui::Button("Erase Picture...", size)) {
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Fadein Screen...", size)) {
          }
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("System")) {
          if (ImGui::Button("Open Menu Screen", size)) {
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Change Battle BGM...", size)) {
          }
          ImGui::SameLine(); // Third column
          if (ImGui::Button("Script...", size)) {
          }
          if (ImGui::Button("Open Save Screen", size)) {
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Change Vehicle BGM...", size)) {
          }
          ImGui::SameLine(); // Third column
          if (ImGui::Button("Plugin Command...", size)) {

          }
          if (ImGui::Button("Change Menu Access...", size)) {
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Change Victory ME...", size)) {
          }
          ImGui::SameLine(); // Third column
          if (ImGui::Button("Game Over", size)) {
          }
          if (ImGui::Button("Change Save Access...", size)) {
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Change Defeat ME...", size)) {
          }
          ImGui::SameLine(); // Third column
          if (ImGui::Button("Return To Title Screen", size)) {
          }
          if (ImGui::Button("Change Map Display Name...", size)) {
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Change Parallax...", size)) {
          }
          if (ImGui::Button("Change Tileset...", size)) {
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Change Actor Images...", size)) {
          }
          if (ImGui::Button("Change Battle Back...", size)) {
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Change Vehicle Image...", size)) {
          }
          if (ImGui::Button("Change Player Followers...", size)) {
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Change Window Color...", size)) {
          }
          if (ImGui::Button("Change Formation Access...", size)) {
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Change Encounter...", size)) {
          }
          ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
      }
    }
    ImGui::EndGroup();


    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape))) {
      m_isNewEntry = false;
      m_selectedCommand = 0;
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  } // End of "Command Window" Popup
}