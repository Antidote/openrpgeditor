#pragma once

#include <map>
#include <string>

#include "Core/Texture.hpp"

#include "Database/Map.hpp"

class ResourceManager {
public:
  ResourceManager() = delete;
  ResourceManager(ResourceManager&)=delete;
  ResourceManager(ResourceManager&&)=delete;
  ResourceManager& operator=(ResourceManager&)=delete;
  ResourceManager& operator=(ResourceManager&&)=delete;
  explicit ResourceManager(std::string_view basePath);
  ~ResourceManager();

  Texture loadImage(std::string_view path);
  Texture loadAnimationImage(std::string_view path);
  Texture loadBattlebacks1Image(std::string_view path);
  Texture loadBattlebacks2Image(std::string_view path);
  Texture loadCharacterImage(std::string_view path);
  Texture loadEnemyImage(std::string_view path);
  Texture loadFaceImage(std::string_view path);
  Texture loadParallaxImage(std::string_view path);
  Texture loadPictureImage(std::string_view path);
  Texture loadSVActorImage(std::string_view path);
  Texture loadSVEnemyImage(std::string_view path);
  Texture loadSystemImage(std::string_view path);
  Texture loadTilesetImage(std::string_view path);
  Texture loadTitle1Image(std::string_view path);
  Texture loadTitle2Image(std::string_view path);

  static ResourceManager* instance() { return m_instance; }

private:
  Texture loadTexture(std::string_view path);
  std::map<std::string, Texture> m_loadedTextures; // Path -> texture
  std::string m_dataPath;
  std::string m_audioPath;
  std::string m_bgmPath;
  std::string m_bgsPath;
  std::string m_sePath;
  std::string m_imgPath;
  std::string m_animationsPath;
  std::string m_battlebacks1Path;
  std::string m_battlebacks2Path;
  std::string m_charactersPath;
  std::string m_enemiesPath;
  std::string m_facesPath;
  std::string m_parallaxesPath;
  std::string m_picturesPath;
  std::string m_sv_actorsPath;
  std::string m_sv_enemiesPath;
  std::string m_systemPath;
  std::string m_tilesetsPath;
  std::string m_titles1Path;
  std::string m_titles2Path;

  static ResourceManager* m_instance;
};