#include "Database/Plugins.hpp"
#include <fstream>

void to_json(nlohmann::ordered_json& to, const Plugin& plugin) {
  to = {{"name", plugin.name}, {"status", plugin.status}, {"description", plugin.description}, {"parameters", nlohmann::json{}}};

  if (!plugin.parameters.empty()) {
    for (const auto& parameter : plugin.parameters) {
      to["parameters"][parameter.first] = parameter.second;
    }
  } else {
    to["parameters"] = nlohmann::ordered_json({});
  }
}

void from_json(const nlohmann::ordered_json& from, Plugin& plugin) {
  plugin.name = from["name"].get<std::string>();
  plugin.status = from["status"].get<bool>();
  plugin.description = from["description"].get<std::string>();
  if (from.contains("parameters")) {
    nlohmann::ordered_json parameters = from["parameters"];
    for (const auto& [name, value] : parameters.items()) {
      plugin.parameters.emplace_back(name, value);
    }
  }
}

Plugins Plugins::load(std::string_view path) {
  std::ifstream file(path.data());
  Plugins ret;
  if (file.is_open()) {
    std::string line;
    std::string content;
    bool found = false;
    while (std::getline(file, line)) {
      if (line.compare("var $plugins =") == 0) {
        found = true;
        continue;
      }

      if (found) {
        content += "\n" + line;
      }
    }
    if (content.back() == ';') {
      content.back() = ' ';
    }
    nlohmann::ordered_json data = nlohmann::ordered_json::parse(content);

    for (auto [_, value] : data.items()) {
      auto& plugin = ret.plugins.emplace_back();
      value.get_to(plugin);
    }
  }

  return ret;
}

bool Plugins::serialize(std::string_view path) {
  std::ofstream file(path.data());

  if (file.is_open()) {
    file << "// Generated by Open RPG Editor.\n";
    file << "// Do not edit this file directly.\n";
    file << "var $plugins =\n";
    file << "[\n";

    for (const auto& plugin : plugins) {
      nlohmann::ordered_json data = plugin;
      file << data.dump(4) << (&plugin != &plugins.back() ? ",\n" : "\n");
    }

    file << "];\n";
    return true;
  }

  return false;
}
