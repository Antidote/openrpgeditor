#include "Dialog_StopSE.hpp"
#include <tuple>

std::tuple<bool, bool> Dialog_StopSE::draw() {
  return std::make_tuple(!m_open, m_confirmed);
}