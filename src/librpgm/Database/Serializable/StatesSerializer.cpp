#include "Database/Serializable/StatesSerializer.hpp"

void StatesSerializer::serialize(std::ofstream& os) const {
  try {
    nlohmann::json data;

    for (const State& state : m_data.states()) {
      if (state.m_isValid) {
        data.push_back(state);
      } else {
        data.push_back(nullptr);
      }
    }

    os << data.dump(4);
  } catch (...) {}
}

void StatesSerializer::deserialize(std::ifstream& is) {
  try {
    nlohmann::json data = nlohmann::json::parse(is);
    m_data.states().reserve(data.size());

    int i = 0;
    for (const auto& [_, value] : data.items()) {
      State& state = m_data.states().emplace_back();
      state.m_isValid = value != nullptr;
      if (state.m_isValid) {
        value.get_to(state);
      } else {
        state.id = i;
      }
      ++i;
    }
  } catch (...) {}
}