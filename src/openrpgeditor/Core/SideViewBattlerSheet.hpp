#pragma once

#include "Core/Texture.hpp"
#include <array>
#include <string_view>

// clang-format off
enum class SideViewActionType {
  Invalid=-1,
  StepForward,     Thrust,       Escape,
  NormalStandby,   Swing,        Victory,
  ChantingStandby, Missile,      NearDeath,
  Guard,           GeneralSkill, StatusAilment,
  Damage,          Magic,        Sleep,
  Evade,           Item,         Dead
};
// clang-format on

struct SideViewActionFrame {
  float u0{};
  float v0{};
  float u1{};
  float v1{};
};

struct SideViewAction {
  SideViewActionType type{SideViewActionType::Invalid};
  std::array<SideViewActionFrame, 3> frames;
};
struct SideViewBattlerSheet {
  explicit SideViewBattlerSheet(const std::string_view sheetName);

  [[nodiscard]] SideViewAction getAction(SideViewActionType actionType) const;
  [[nodiscard]] const Texture& texture() const { return m_texture; }

  [[nodiscard]] int characterWidth() const { return m_texture.width() / 9; }
  [[nodiscard]] int characterHeight() const { return m_texture.height() / 6; }

private:
  Texture m_texture;
};
