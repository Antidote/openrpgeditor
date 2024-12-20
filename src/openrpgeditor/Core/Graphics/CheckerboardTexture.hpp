#pragma once

#include "SDL2/SDL.h"
#include "imgui.h"
enum class CellSizes : uint8_t {
  _8 = 8,
  _16 = 16,
  _24 = 24,
  _32 = 32,
  _48 = 48,
  _64 = 64,
};
struct CheckerboardTexture {

  CheckerboardTexture() = default;
  CheckerboardTexture(int width, int height, CellSizes cellSize = CellSizes::_64, uint8_t evenColor = 255, uint8_t oddColor = 220);
  CheckerboardTexture(const CheckerboardTexture& other);
  CheckerboardTexture& operator=(const CheckerboardTexture& other);
  ~CheckerboardTexture();

  [[nodiscard]] void* get() const { return m_texture; }
  explicit operator bool() const { return m_texture != nullptr; }

  operator ImTextureID() const { return reinterpret_cast<ImTextureID>(m_texture); }

  int width() const { return m_width; }
  int height() const { return m_height; }

private:
  mutable void* m_texture = nullptr;
  int m_width;
  int m_height;
};
