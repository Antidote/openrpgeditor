
#include "CommonEvents.hpp"
#include <fstream>

using json = nlohmann::json;

void to_json(nlohmann::json& to, const CommonEvent& event) {
  CommandParser parser;
  json commands;
  parser.serialize(commands, event.commands);

  to = json{{"id", event.id},
            {"list", commands},
            {"name", event.name},
            {"switchId", event.switchId},
            {"trigger", event.trigger}};
}
void from_json(const nlohmann::json& to, CommonEvent& event) {
  to.at("id").get_to(event.id);
  to.at("name").get_to(event.name);
  CommandParser parser;
  event.commands = parser.parse(to.at("list"));
  to.at("switchId").get_to(event.switchId);
  to.at("trigger").get_to(event.trigger);
}

CommonEvents CommonEvents::load(std::string_view filepath) {
  std::ifstream file(filepath.data());
  json data = json::parse(file);
  CommonEvents commonEvents;
  commonEvents.m_events.reserve(data.size());

  for (const auto& [_, value] : data.items()) {
    value.get_to(commonEvents.m_events.emplace_back());
  }
  return commonEvents;
}

bool CommonEvents::serialize(std::string_view filename) {
  // TODO: Temporary file for atomic writes

  std::ofstream file(filename.data());
  json data;

  for (const auto& event : m_events) {
    data.push_back(event);
  }

  file << data.dump(4);

  return true;
}