#include "../include/Character.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "../include/utils.h"

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

bool Character::LoadFromFile(const std::string& file_path) {
  std::ifstream file(file_path);
  if (!file.is_open()) {
    return false;
  }

  std::string line;
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string key;
    if (std::getline(iss, key, '=')) {
      std::string value;
      if (std::getline(iss, value)) {
        utils::Trim(key);
        utils::Trim(value);

        if (key == "name") {
          name_ = value;
        } else if (key == "class") {
          class_ = value;
        } else if (key == "level") {
          level_ = std::stoi(value);
        } else if (key == "health") {
          health_ = std::stoi(value);
        } else if (key == "max_health") {
          max_health_ = std::stoi(value);
        } else if (key == "attack") {
          attack_ = std::stoi(value);
        } else if (key == "defense") {
          defense_ = std::stoi(value);
        } else if (key == "gold") {
          gold_ = std::stoi(value);
        } else if (key == "experience") {
          experience_ = std::stoi(value);
        } else if (key == "experience_to_next_level") {
          experience_to_next_level_ = std::stoi(value);
        } else if (key.find("item_") == 0) {
          std::string item_name = key.substr(5);
          int quantity = std::stoi(value);
          inventory_[item_name] = quantity;
        }
      }
    }
  }

  return true;
}

bool Character::SaveToFile(const std::string& file_path) const {
  std::ofstream file(file_path);
  if (!file.is_open()) {
    return false;
  }

  file << "name = " << name_ << "\n";
  file << "class = " << class_ << "\n";
  file << "level = " << level_ << "\n";
  file << "health = " << health_ << "\n";
  file << "max_health = " << max_health_ << "\n";
  file << "attack = " << attack_ << "\n";
  file << "defense = " << defense_ << "\n";
  file << "gold = " << gold_ << "\n";
  file << "experience = " << experience_ << "\n";
  file << "experience_to_next_level = " << experience_to_next_level_ << "\n";

  for (const auto& item : inventory_) {
    file << "item_" << item.first << " = " << item.second << "\n";
  }

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
  if (quantity <= 0) return;
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

void Character::LoadClass(const std::string& file_path) {
  std::ifstream file(file_path);
  if (!file.is_open()) {
    abort();
  }
  std::string line;
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string key;
    if (std::getline(iss, key, '=')) {
      std::string value;
      if (std::getline(iss, value)) {
        utils::Trim(key);
        utils::Trim(value);

        if (key == "max_health") {
          SetMaxHp(std::stoi(value));
          AddHealth(max_health_-health_);
        } else if (key == "attack") {
          SetAttack(std::stoi(value));
        }else if (key == "defense"){
          SetDefense(std::stoi(value));
        }else if (key == "gold"){
          SetGold(std::stoi(value));
        }else if (key == "item_Health Potion") {
          AddItem("Health Potion", std::stoi(value));
        }
      }
    }
  }
}

void Character::SetClass(const std::string &character_class) {
  class_ = character_class;
  if (character_class == "Warrior") {
    LoadClass("../data/warrior_data.txt");
  }else if (character_class == "Medic") {
    LoadClass("../data/medic_data.txt");
  }else if (character_class == "Mechanic") {
    LoadClass("../data/mechanic_data.txt");
  }else if (character_class == "Huckster") {
    LoadClass("../data/huckster_data.txt");
  }


}

}  // namespace rpg