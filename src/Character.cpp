#include "../include/Character.h"

#include <algorithm>
#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <stdexcept>

#include "../include/utils.h"
using json = nlohmann::json;

namespace rpg {

namespace {

constexpr int kBaseExpToLevel = 100;
constexpr int kHealthPerLevel = 10;
constexpr int kAttackPerLevel = 2;
constexpr int kDefensePerLevel = 1;

}  // namespace

Character::Character()
    : name_("Unknown"),
      class_("Adventurer"),
      level_(1),
      health_(20),
      max_health_(20),
      attack_(5),
      defense_(3),
      gold_(0),
      experience_(0),
      experience_to_next_level_(kBaseExpToLevel) {
  CalculateStats();
}

Character::Character(const std::string& file_path) : Character() {
  LoadFromFile(file_path);
}

bool Character::LoadFromFile(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) return false;

  json data;
  try {
    file >> data;

    name_ = data["name"];
    class_ = data["class"];
    level_ = data["level"];
    health_ = data["health"];
    max_health_ = data["max_health"];
    attack_ = data["attack"];
    defense_ = data["defense"];
    gold_ = data["gold"];
    experience_ = data["experience"];
    permanent_attack_bonus_ = data["permanent_attack_bonus"];
    permanent_defense_bonus_ = data["permanent_defense_bonus"];

    inventory_.clear();
    for (auto& [key, value] : data["inventory"].items()) {
      inventory_[key] = value;
    }

    purchased_upgrades_.clear();
    for (auto& item : data["purchased_upgrades"]) {
      purchased_upgrades_.push_back(item);
    }

    return true;
  } catch (...) {
    return false;
  }
}

bool Character::SaveToFile(const std::string& filename) const {
  json data;

  data["name"] = name_;
  data["class"] = class_;
  data["level"] = level_;
  data["health"] = health_;
  data["max_health"] = max_health_;
  data["attack"] = attack_;
  data["defense"] = defense_;
  data["gold"] = gold_;
  data["experience"] = experience_;
  data["permanent_attack_bonus"] = permanent_attack_bonus_;
  data["permanent_defense_bonus"] = permanent_defense_bonus_;

  data["inventory"] = json::object();
  for (const auto& item : inventory_) {
    data["inventory"][item.first] = item.second;
  }

  data["purchased_upgrades"] = purchased_upgrades_;

  std::ofstream file(filename);
  if (!file.is_open()) return false;

  file << data.dump(2);
  return true;
}

void Character::AddHealth(int value) {
  health_ += value;
  if (health_ > max_health_) {
    health_ = max_health_;
  } else if (health_ < 0) {
    health_ = 0;
  }
}

void Character::AddExperience(int value) {
  experience_ += value;
  while (experience_ >= experience_to_next_level_) {
    experience_ -= experience_to_next_level_;
    LevelUp();
  }
}

void Character::LevelUp() {
  ++level_;
  experience_to_next_level_ = kBaseExpToLevel * level_;
  CalculateStats();
  health_ = max_health_;  // Полное исцеление при повышении уровня
}

void Character::CalculateStats() {
  max_health_ += (level_ - 1) * kHealthPerLevel;
  attack_ += (level_ - 1) * kAttackPerLevel;
  defense_ += (level_ - 1) * kDefensePerLevel;
}

void Character::AddItem(const std::string& item, int quantity) {
  inventory_[item] += quantity;
}

bool Character::UseItem(const std::string& item) {
  auto it = inventory_.find(item);
  if (it == inventory_.end() || it->second <= 0) {
    return false;
  }

  if (--it->second <= 0) {
    inventory_.erase(it);
  }
  return true;
}

int Character::GetItemCount(const std::string& item) const {
  auto it = inventory_.find(item);
  return it != inventory_.end() ? it->second : 0;
}

void Character::SetClass(const std::string& character_class) {
  class_ = character_class;
}

}  // namespace rpg