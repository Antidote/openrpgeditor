#include "Core/Graphics/CharacterSheet.hpp"

#include "Core/ResourceManager.hpp"

CharacterSheet::CharacterSheet(std::string_view sheetName) : m_characterName(sheetName) {
  if (sheetName.empty()) {
    return;
  }

  int count = 0;
  for (auto c : sheetName) {
    if (count >= 2) {
      break;
    }

    if (c == '$') {
      m_isSingleCharacter = true;
      ++count;
    } else if (c == '!') {
      m_noShift6_noHalfTransInBush = true;
      ++count;
    }
  }

  m_sheetTexture = ResourceManager::instance()->loadCharacterImage(sheetName);
}

int CharacterSheet::characterWidth() const { return m_sheetTexture.width() / patternCount(); }
int CharacterSheet::characterHeight() const { return m_sheetTexture.height() / directionCount(); }

SimpleRect CharacterSheet::getRectForCharacter(int character, int pattern, Direction direction) const {
  if (!m_sheetTexture) {
    return {};
  }
  const int CharacterSpriteWidth = characterWidth();
  const int CharacterSpriteHeight = characterHeight();
  const int CharacterAtlasWidth = m_isSingleCharacter ? m_sheetTexture.width() : m_sheetTexture.width() / 4;
  const int CharacterAtlasHeight = m_isSingleCharacter ? m_sheetTexture.height() : m_sheetTexture.height() / 2;
  const float charX = static_cast<float>((character % (m_sheetTexture.width() / CharacterAtlasWidth)) * CharacterAtlasWidth);
  const float charY = static_cast<float>((character / (m_sheetTexture.width() / CharacterAtlasWidth)) * CharacterAtlasHeight);
  const float patternOffset = static_cast<float>(pattern * CharacterSpriteWidth);
  const float directionOffset = direction != Direction::Retain ? (((static_cast<int>(direction) - 2) / 2) * CharacterSpriteHeight) : 0.f;

  return {
      PointF{(charX + patternOffset) / m_sheetTexture.width(), (charY + directionOffset) / m_sheetTexture.height()},
      PointF{(charX + patternOffset + CharacterSpriteWidth) / m_sheetTexture.width(), (charY + directionOffset + CharacterSpriteHeight) / m_sheetTexture.height()},
  };
}
