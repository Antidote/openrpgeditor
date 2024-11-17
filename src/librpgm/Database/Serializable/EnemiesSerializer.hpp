#pragma once
#include "Database/Serializable/ISerializable.hpp"
#include "Database/Enemies.hpp"

class EnemiesSerializer final : public ITypedSerializable<Enemies> {
public:
  explicit EnemiesSerializer(const std::string_view filepath) : ITypedSerializable(filepath) {}
  EnemiesSerializer(const Enemies& enemies, const std::string_view filepath) : ITypedSerializable(enemies, filepath) {}
  void serialize(std::ofstream& os) const override;
  void deserialize(std::ifstream& is) override;
};