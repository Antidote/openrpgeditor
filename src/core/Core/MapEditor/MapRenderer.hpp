#pragma once

#include <vector>
#include <algorithm>

#include "../Texture.hpp"
#include "../../../librpgm/Database/Map.hpp"

#include "../../../librpgm/Database/Tilesets.hpp"

class Map;
class Tileset;
struct SDL_Texture;

struct TileRect {
  int tileId;
  int setNum;
  int u;
  int v;
  int x;
  int y;
  int tileWidth;
  int tileHeight;
  int animX;
  int animY;
};

class MapRenderer {
public:
  // struct MapLayer {
  //   SDL_Texture* bitmap;
  //
  //   std::vector<TileRect> rects;
  //   void addRect(float u, float v, float x, float y, int tileWidth, int tileHeight) {
  //     rects.push_back({u, v, x, y, tileWidth, tileHeight});
  //   }
  // };

  static constexpr int TILE_ID_B = 0;
  static constexpr int TILE_ID_C = 256;
  static constexpr int TILE_ID_D = 512;
  static constexpr int TILE_ID_E = 768;
  static constexpr int TILE_ID_A5 = 1356;
  static constexpr int TILE_ID_A1 = 2048;
  static constexpr int TILE_ID_A2 = 2816;
  static constexpr int TILE_ID_A3 = 4352;
  static constexpr int TILE_ID_A4 = 5888;
  static constexpr int TILE_ID_MAX = 8192;

  MapRenderer();
  void setMap(const Map* map, const Tileset* tilest, int tileWidth = 48, int tileHeight = 48);

  void update();

  int tileId(int x, int y, int z) const;
  bool isOverworld() const { return m_tileset && m_tileset->mode == Tileset::Mode::World; }

  [[nodiscard]] std::vector<int> tilesetFlags() const {
    if (m_tileset) {
      return m_tileset->flags;
    }
    return {};
  }

  [[nodiscard]] std::vector<int> layeredTiles(const int x, const int y) const;
  [[nodiscard]] std::vector<int> allTiles(const int x, const int y) const;
  [[nodiscard]] int autoTileType(const int x, const int y, const int z) const;
  [[nodiscard]] bool checkPassage(const int x, const int y, const int bit) const;
  [[nodiscard]] bool isPassable(const int x, const int y, const int d) const;

  [[nodiscard]] static bool isVisibleTile(int tileId) { return tileId > 0 && tileId < TILE_ID_MAX; }
  [[nodiscard]] static bool isAutoTile(int tileId) { return tileId >= TILE_ID_A1; }
  [[nodiscard]] static int getAutoTileKind(int tileId) { return std::floor((tileId - TILE_ID_A1) / 48); }
  [[nodiscard]] static int getAutoTileShape(int tileId) { return (tileId - TILE_ID_A1) % 48; }
  [[nodiscard]] static int makeAutoTileId(int kind, int shape) { return TILE_ID_A1 + kind * 48 + shape; }
  [[nodiscard]] static bool isSameKindTile(int tileId1, int tileId2) {
    if (isAutoTile(tileId1) && isAutoTile(tileId2)) {
      return getAutoTileKind(tileId1) == getAutoTileKind(tileId2);
    }

    return tileId1 == tileId2;
  }
  [[nodiscard]] static bool isTileA1(int tileId) { return tileId >= TILE_ID_A1 && tileId < TILE_ID_A2; }
  [[nodiscard]] static bool isTileA2(int tileId) { return tileId >= TILE_ID_A2 && tileId < TILE_ID_A3; }
  [[nodiscard]] static bool isTileA3(int tileId) { return tileId >= TILE_ID_A3 && tileId < TILE_ID_A4; }
  [[nodiscard]] static bool isTileA4(int tileId) { return tileId >= TILE_ID_A4 && tileId < TILE_ID_MAX; }
  [[nodiscard]] static bool isTileA5(int tileId) { return tileId >= TILE_ID_A5 && tileId < TILE_ID_A1; }
  [[nodiscard]] static bool isWaterTile(int tileId) {
    if (isTileA1(tileId)) {
      return !(tileId >= TILE_ID_A1 + 96 && tileId < TILE_ID_A1 + 192);
    }
    return false;
  }
  [[nodiscard]] static bool isWaterfallTile(int tileId) {
    if (tileId >= TILE_ID_A1 + 192 && tileId < TILE_ID_A2) {
      return getAutoTileKind(tileId) % 2 == 1;
    }
    return false;
  }

  [[nodiscard]] static bool isGroundTile(int tileId) {
    return isTileA1(tileId) || isTileA2(tileId) || isTileA5(tileId);
  }
  [[nodiscard]] static bool isShadowingTile(int tileId) { return isTileA3(tileId) || isTileA4(tileId); }
  [[nodiscard]] static bool isRoofTile(int tileId) { return isTileA3(tileId) && getAutoTileKind(tileId) % 16 < 8; }
  [[nodiscard]] static bool isWallTopTile(int tileId) { return isTileA4(tileId) && getAutoTileKind(tileId) % 16 < 8; }
  [[nodiscard]] static bool isWallSideTile(int tileId) {
    return (isTileA3(tileId) || isTileA4(tileId)) && getAutoTileKind(tileId) % 16 >= 8;
  }
  [[nodiscard]] static bool isWallTile(int tileId) { return isWallTopTile(tileId) || isWallSideTile(tileId); }
  [[nodiscard]] static bool isFloorTypeAutotile(int tileId) {
    return (isTileA1(tileId) && !isWaterfallTile(tileId)) || isTileA2(tileId) || isWallTopTile(tileId);
  }
  [[nodiscard]] static bool isWallTypeAutotile(int tileId) { return isRoofTile(tileId) || isWallSideTile(tileId); }
  [[nodiscard]] static bool isWaterfallTypeAutotile(int tileId) { return isWaterfallTile(tileId); }

  [[nodiscard]] int tileIdFromCoords(int x, int y, int z) { return (z * m_map->width + y) * m_map->width + x; }

  [[nodiscard]] bool isHigherTile(int tileId) { return m_tileset->flags[tileId] & 0x10; }

  [[nodiscard]] bool isTableTile(int tileId) { return isTileA2(tileId) && m_tileset->flags[tileId] & 0x80; }

  [[nodiscard]] bool isOverpassPosition(int mx, int my) { return false; }

  /* TODO: These are nightmares taken from the original javascript, these needs to be rewritten so it's not a
   * monstrosity */
#if 0
  std::vector<int>& readLastTiles(int i, int x, int y) {
    if (i < m_lastTiles.size()) {
      if (y < m_lastTiles[i].size()) {
        if (x < m_lastTiles[i][y].size()) {
          return m_lastTiles[i][y][x];
        }
        m_lastTiles[i][y].emplace_back();
        return m_lastTiles[i][y][x];
      }
      return m_lastTiles[i].emplace_back().emplace_back();
    }
    return m_lastTiles.emplace_back().emplace_back().emplace_back();
  }

  void writeLastTiles(int i, int x, int y, const std::vector<int>& tiles) {
    return;
    if (i < m_lastTiles.size()) {
      if (y < m_lastTiles[i].size()) {
        if (x < m_lastTiles[i][y].size()) {
          m_lastTiles[i][y][x] = tiles;
        } else {
          m_lastTiles[i][y].push_back(tiles);
        }
      } else {
        m_lastTiles[i].emplace_back().emplace_back(tiles);
      }
    } else {
      m_lastTiles.emplace_back().emplace_back().emplace_back(tiles);
    }
  }

#endif
  SDL_Texture* getLowerBitmap() const { return m_lowerBitmap; }
  SDL_Texture* getUpperBitmap() const { return m_upperBitmap; }


  void getTileRect(std::vector<TileRect>& layer, int tileId, int dx, int dy);
  std::array<Texture, 9> m_tilesetTextures;

private:
  bool m_frameUpdated = true;
  void drawTile(std::vector<TileRect>& bitmap, int tileId, int dx, int dy);
  void drawAutoTile(std::vector<TileRect>& layer, int tileId, int dx, int dy);
  void drawNormalTile(std::vector<TileRect>& layer, int tileId, int dx, int dy) const;
  void beginBlit(SDL_Texture* source);
  void blitImage(SDL_Texture* bitmap, int sx, int sy, int sw, int sh, int dx, int dy, int dw, int dh);
  void endBlit();
  void clearRect(SDL_Texture* clr, int x, int y, int w, int h);

  const Map* m_map = nullptr;
  const Tileset* m_tileset = nullptr;
  bool m_isValid = false;
  int m_tileWidth{48};
  int m_tileHeight{48};
  SDL_Texture* m_lowerBitmap = nullptr;
  SDL_Texture* m_upperBitmap = nullptr;
  SDL_Texture* m_oldTarget = nullptr;
  std::vector<int> m_lowerTiles;
  std::vector<int> m_upperTiles;
  std::vector<std::vector<std::vector<std::vector<int>>>> m_lastTiles;
};