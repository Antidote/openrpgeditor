#include "Database/GameConstants.hpp"

#include "Database/Database.hpp"

#include <format>
#include <fstream>

void to_json(nlohmann::ordered_json& json, const GameConstants& constants) {
  json = {
      {"variables", constants.variables},   {"switches", constants.switches},     {"actors", constants.actors},     {"classes", constants.classes},           {"skills", constants.skills},
      {"items", constants.items},           {"weapons", constants.weapons},       {"armors", constants.armors},     {"enemies", constants.enemies},           {"troops", constants.troops},
      {"states", constants.states},         {"animations", constants.animations}, {"tilesets", constants.tilesets}, {"commonEvents", constants.commonEvents}, {"maps", constants.maps},
      {"generateJS", constants.generateJS},
  };
}
void from_json(const nlohmann::ordered_json& json, GameConstants& constants) {
  constants.variables = json.value("variables", constants.variables);
  constants.switches = json.value("switches", constants.switches);
  constants.actors = json.value("actors", constants.actors);
  constants.classes = json.value("classes", constants.classes);
  constants.skills = json.value("skills", constants.skills);
  constants.items = json.value("items", constants.items);
  constants.weapons = json.value("weapons", constants.weapons);
  constants.armors = json.value("armors", constants.armors);
  constants.enemies = json.value("enemies", constants.enemies);
  constants.troops = json.value("troops", constants.troops);
  constants.states = json.value("states", constants.states);
  constants.animations = json.value("animations", constants.animations);
  constants.tilesets = json.value("tilesets", constants.tilesets);
  constants.commonEvents = json.value("commonEvents", constants.commonEvents);
  constants.maps = json.value("maps", constants.maps);
  constants.generateJS = json.value("generateJS", constants.generateJS);
}

GameConstants GameConstants::load(std::string_view path) {
  if (std::ifstream file(path.data()); file.is_open()) {
    try {
      nlohmann::json data = nlohmann::ordered_json::parse(file);
      GameConstants ret;
      data.get_to(ret);
      return ret;
    } catch (...) {}
  }

  return {};
}

bool GameConstants::serialize(const std::string_view path) {
  std::ofstream file(path.data());
  try {
    if (file.is_open()) {
      const nlohmann::ordered_json data = *this;
      file << data.dump(4);
      return true;
    }
  } catch (...) {}
  return false;
}

bool GameConstants::generateConstantsJS(std::string_view path) {
  std::ofstream file(path.data());
  if (!file.is_open()) {
    return false;
  }

  file << "// Generated by OpenRPGEditor\n";
  file << "// DO NOT MODIFY!\n";
  file << "\n/* -----------------VARIABLES----------------- */\n";
  for (const auto& [id, alias] : variables) {
    file << std::format(
        "\n"
        "/*\n"
        " * @name VAR_{}\n"
        " * @summary Exported Variable \"{}\" ({})\n"
        " * @readonly\n"
        " */\n",
        alias, Database::instance()->system.variables[id], id);
    file << std::format("const VAR_{} = {};\n", alias, id);
  }
  file << "\n/* -----------------SWITCHES------------------ */\n";
  for (const auto& [id, alias] : switches) {
    file << std::format(
        "\n"
        "/*\n"
        " * @name SW_{}\n"
        " * @summary Exported Switch \"{}\" ({})\n"
        " * @readonly\n"
        " */\n",
        alias, Database::instance()->system.switches[id], id);
    file << std::format("const SW_{} = {};\n", alias, id);
  }
  file << "\n/* ------------------ACTORS------------------- */\n";
  for (const auto& [id, alias] : actors) {
    auto act = Database::instance()->actors.actor(id);
    file << std::format(
        "\n"
        "/*\n"
        " * @name ACT_{}\n"
        " * @summary Exported Actor \"{}\" ({})\n"
        " * @readonly\n",
        alias, act->name, id);
    if (!act->note.empty()) {
      auto lines = splitString(act->note, '\n');
      file << " * @description\n";
      for (auto line : lines) {
        file << std::format(" * {}\n", line);
      }
    }
    if (!act->profile.empty()) {
      auto lines = splitString(act->profile, '\n');
      file << " * ---- PROFILE ----\n";
      for (auto line : lines) {
        file << std::format(" * {}\n", line);
      }
    }
    file << " */\n";
    file << std::format("const ACT_{} = {};\n", alias, id);
  }

  file << "\n/* ------------------CLASSES------------------ */\n";
  for (const auto& [id, alias] : classes) {
    auto cls = Database::instance()->classes.classType(id);
    file << std::format(
        "\n"
        "/*\n"
        " * @name CLS_{}\n"
        " * @summary Exported Class \"{}\" ({})\n"
        " * @readonly\n",
        alias, cls->name, id);
    if (!cls->note.empty()) {
      auto lines = splitString(cls->note, '\n');
      file << " * @description\n";
      for (auto line : lines) {
        file << std::format(" * {}\n", line);
      }
    }
    file << " */\n";
    file << std::format("const CLS_{} = {};\n", alias, id);
  }

  file << "\n/* ------------------SKILLS------------------- */\n";
  for (const auto& [id, alias] : skills) {
    auto skill = Database::instance()->skills.skill(id);
    file << std::format(
        "\n"
        "/*\n"
        " * @name SKL_{}\n"
        " * @summary Exported Skill \"{}\" ({})\n"
        " * @readonly\n",
        alias, skill->name, id);
    if (!skill->note.empty()) {
      auto lines = splitString(skill->note, '\n');
      file << " * @description\n";
      for (auto line : lines) {
        file << std::format(" * {}\n", line);
      }
    }
    file << " */\n";
    file << std::format("const SKL_{} = {};\n", alias, id);
  }

  file << "\n/* ------------------ITEMS-------------------- */\n";
  for (const auto& [id, alias] : items) {
    auto itm = Database::instance()->items.item(id);
    file << std::format(
        "\n"
        "/*\n"
        " * @name ITM_{}\n"
        " * @summary Exported Item \"{}\" ({})\n"
        " * @readonly\n",
        alias, itm->name, id);
    if (!itm->note.empty()) {
      auto lines = splitString(itm->note, '\n');
      file << " * @description\n";
      for (auto line : lines) {
        file << std::format(" * {}\n", line);
      }
    }
    file << " */\n";
    file << std::format("const ITM_{} = {};\n", alias, id);
  }

  file << "\n/* ------------------WEAPONS------------------ */\n";
  for (const auto& [id, alias] : weapons) {
    auto wpn = Database::instance()->weapons.weapon(id);
    file << std::format(
        "\n"
        "/*\n"
        " * @name WPN_{}\n"
        " * @summary Exported Weapon \"{}\" ({})\n"
        " * @readonly\n",
        alias, wpn->name, id);
    if (!wpn->note.empty()) {
      auto lines = splitString(wpn->note, '\n');
      file << " * @description\n";
      for (auto line : lines) {
        file << std::format(" * {}\n", line);
      }
    }
    file << " */\n";
    file << std::format("const WPN_{} = {};\n", alias, id);
  }
  file << "\n/* ------------------ARMORS------------------- */\n";
  for (const auto& [id, alias] : armors) {
    auto arm = Database::instance()->armors.armor(id);
    file << std::format(
        "\n"
        "/* @name ARM_{}\n"
        " * @summary Exported Armor \"{}\" ({})\n"
        " * @readonly\n",
        alias, arm->name, id);
    if (!arm->note.empty()) {
      auto lines = splitString(arm->note, '\n');
      file << " * @description\n";
      for (auto line : lines) {
        file << std::format(" * {}\n", line);
      }
    }
    file << " */\n";
    file << std::format("const ARM_{} = {};\n", alias, id);
  }
  file << "\n/* ------------------ENEMIES------------------ */\n";
  for (const auto& [id, alias] : enemies) {
    auto enm = Database::instance()->enemies.enemy(id);
    file << std::format(
        "\n"
        "/*\n"
        " * @name ENM_{}\n"
        " * @summary Exported Enemy \"{}\" ({})\n"
        " * @readonly\n",
        alias, enm->name, id);
    if (!enm->note.empty()) {
      auto lines = splitString(enm->note, '\n');
      file << " * @description\n";
      for (auto line : lines) {
        file << std::format(" * {}\n", line);
      }
    }
    file << " */\n";
    file << std::format("const ENM_{} = {};\n", alias, id);
  }
  file << "\n/* ------------------TROOPS------------------- */\n";
  for (const auto& [id, alias] : troops) {
    auto trp = Database::instance()->troops.troop(id);
    file << std::format(
        "\n"
        "/*\n"
        " * @name TRP_{}\n"
        " * @summary Exported Troop \"{}\" ({})\n"
        " * @readonly\n"
        " */\n",
        alias, trp->name, id);
    file << std::format("const TRP_{} = {};\n", alias, id);
  }
  file << "\n/* ------------------STATES------------------- */\n";
  for (const auto& [id, alias] : states) {
    auto sta = Database::instance()->states.state(id);
    file << std::format(
        "\n"
        "/*\n"
        " * @name STA_{}\n"
        " * @summary Exported State \"{}\" ({})\n"
        " * @readonly\n",
        alias, sta->name, id);
    if (!sta->note.empty()) {
      auto lines = splitString(sta->note, '\n');
      file << " * @description\n";
      for (auto line : lines) {
        file << std::format(" * {}\n", line);
      }
    }
    file << " */\n";
    file << std::format("const STA_{} = {};\n", alias, id);
  }
  file << "\n/* -----------------ANIMATIONS------------------ */\n";
  for (const auto& [id, alias] : animations) {
    auto tls = Database::instance()->animations.animation(id);
    file << std::format(
        "\n"
        "/*\n"
        " * @name ANM_{}\n"
        " * @summary Exported Animation \"{}\" ({})\n"
        " * @readonly\n"
        " */\n",
        alias, tls->name, id);
    file << std::format("const ANM_{} = {};\n", alias, id);
  }
  file << "\n/* -----------------TILESETS------------------ */\n";
  for (const auto& [id, alias] : tilesets) {
    auto tls = Database::instance()->tilesets.tileset(id);
    file << std::format(
        "\n"
        "/*\n"
        " * @name TLS_{}\n"
        " * @summary Exported Tileset \"{}\" ({})\n"
        " * @readonly\n",
        alias, tls->name, id);
    if (!tls->note.empty()) {
      auto lines = splitString(tls->note, '\n');
      file << " * @description\n";
      for (auto line : lines) {
        file << std::format(" * {}\n", line);
      }
    }
    file << " */\n";
    file << std::format("const TLS_{} = {};\n", alias, id);
  }

  file << "\n/* ---------------COMMON EVENTS--------------- */\n";
  for (const auto& [id, alias] : commonEvents) {
    auto ce = Database::instance()->commonEvents.event(id);
    file << std::format(
        "\n"
        "/*\n"
        " * @name CMN_{}\n"
        " * @summary Exported Common Event \"{}\" ({})\n"
        " * @readonly\n"
        " */\n",
        alias, ce->name, id);
    file << std::format("const CMN_{} = {};\n", alias, id);
  }

  file << "\n/* -------------------MAPS-------------------- */\n";
  for (const auto& [id, alias] : maps) {
    auto map = Database::instance()->mapInfos.map(id);
    file << std::format(
        "\n"
        "/*\n"
        " * @name MAP_{}\n"
        " * @summary Exported Map \"{}\" ({})\n"
        " * @readonly\n",
        alias, map->name, id);
    if (!map->map()->note.empty()) {
      auto lines = splitString(map->map()->note, '\n');
      file << " * @description\n";
      for (auto line : lines) {
        file << std::format(" * {}\n", line);
      }
    }
    file << " */\n";
    file << std::format("const MAP_{} = {};\n", alias, id);
  }
  return true;
}

bool GameConstants::isValidName(Type type, int id, const std::string& constant) {
  if (constant.empty()) {
    return false;
  }
  switch (type) {
  case Type::Variable:
    return std::find_if(variables.begin(), variables.end(), [&constant, id](const auto& v) { return v.first != id && v.second == constant; }) == variables.end();
  case Type::Switch:
    return std::find_if(switches.begin(), switches.end(), [&constant, id](const auto& v) { return v.first != id && v.second == constant; }) == switches.end();
  case Type::Actor:
    return std::find_if(actors.begin(), actors.end(), [&constant, id](const auto& v) { return v.first != id && v.second == constant; }) == actors.end();
  case Type::Class:
    return std::find_if(classes.begin(), classes.end(), [&constant, id](const auto& v) { return v.first != id && v.second == constant; }) == classes.end();
  case Type::Skill:
    return std::find_if(skills.begin(), skills.end(), [&constant, id](const auto& v) { return v.first != id && v.second == constant; }) == skills.end();
  case Type::Item:
    return std::find_if(items.begin(), items.end(), [&constant, id](const auto& v) { return v.first != id && v.second == constant; }) == items.end();
  case Type::Weapon:
    return std::find_if(weapons.begin(), weapons.end(), [&constant, id](const auto& v) { return v.first != id && v.second == constant; }) == weapons.end();
  case Type::Armor:
    return std::find_if(armors.begin(), armors.end(), [&constant, id](const auto& v) { return v.first != id && v.second == constant; }) == armors.end();
  case Type::Enemy:
    return std::find_if(enemies.begin(), enemies.end(), [&constant, id](const auto& v) { return v.first != id && v.second == constant; }) == enemies.end();
  case Type::Troop:
    return std::find_if(troops.begin(), troops.end(), [&constant, id](const auto& v) { return v.first != id && v.second == constant; }) == troops.end();
  case Type::State:
    return std::find_if(states.begin(), states.end(), [&constant, id](const auto& v) { return v.first != id && v.second == constant; }) == states.end();
  case Type::Animation:
    return std::find_if(animations.begin(), animations.end(), [&constant, id](const auto& v) { return v.first != id && v.second == constant; }) == animations.end();
  case Type::Tileset:
    return std::find_if(tilesets.begin(), tilesets.end(), [&constant, id](const auto& v) { return v.first != id && v.second == constant; }) == tilesets.end();
  case Type::CommonEvent:
    return std::find_if(commonEvents.begin(), commonEvents.end(), [&constant, id](const auto& v) { return v.first != id && v.second == constant; }) == commonEvents.end();
  case Type::Map:
    return std::find_if(maps.begin(), maps.end(), [&constant, id](const auto& v) { return v.first != id && v.second == constant; }) == maps.end();
  }

  return false;
}
