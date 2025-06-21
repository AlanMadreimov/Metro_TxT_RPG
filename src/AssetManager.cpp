#include "AssetManager.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <random>
#include <sstream>

#include "../include/utils.h"
using json = nlohmann::json;

namespace rpg {

AssetManager::AssetManager() {}

void AssetManager::LoadCharacterClasses() {
  std::ifstream file("../data/character_classes.json");
  if (!file.is_open()) {
    throw std::runtime_error(
        "Cannot open JSON file: ../data/character_classes.json");
  }
  json data;
  file >> data;

  for (auto& [name, stats] : data.items()) {
    CharacterClassData class_data;
    class_data.base_attack = stats["base_attack"];
    class_data.base_defense = stats["base_defense"];
    class_data.base_health = stats["base_health"];
    class_data.starting_gold = stats["starting_gold"];

    for (auto& [item, count] : stats["starting_items"].items()) {
      class_data.starting_items[item] = count;
    }

    character_classes_[name] = class_data;
  }
}

void AssetManager::LoadNumericData() {
  std::ifstream file("../data/some_numeric.json");
  if (!file.is_open()) {
    throw std::runtime_error(
        "Cannot open JSON file: ../data/some_numeric.json");
  }
  json data;
  file >> data;
  for (auto& [key, value] : data.items()) {
    numeric_.insert({key, value});
  }
}

int AssetManager::GetNum(const std::string& key) {
  auto it = numeric_.find(key);
  if (it == numeric_.end()) {
    throw std::runtime_error("Num key not found: " + key);
  }
  return it->second;
}

// Text
void AssetManager::LoadEnemyData() {
  std::ifstream file("../data/enemy_data.json");
  if (!file.is_open()) {
    throw std::runtime_error("Cannot open JSON file: ../data/enemy_data.json");
  }
  json data;
  file >> data;
  EnemyData enemy;
  for (auto enemy_cur : data.items()) {
    enemy.name = enemy_cur.key();
    enemy.base_health = enemy_cur.value()[0];
    enemy.base_attack = enemy_cur.value()[1];
    enemy.base_defense = enemy_cur.value()[2];
    enemy.exp_reward = enemy_cur.value()[3];
    enemy.gold_reward = enemy_cur.value()[4];
    enemies_.push_back(enemy);
  }
}

void AssetManager::LoadMenuData() {
  std::ifstream file("../data/menus_data.json");
  if (!file.is_open()) {
    throw std::runtime_error("Cannot open JSON file: ../data/menus_data.json");
  }
  json data;
  file >> data;
  for (auto& [name, options] : data.items()) {
    menus_.insert({name, options});
  }
}

void AssetManager::LoadTextData() {
  std::ifstream file("../data/texts_data.json");
  if (!file.is_open()) {
    throw std::runtime_error("Cannot open JSON file: ../data/texts_data.json");
  }
  json data;
  file >> data;
  for (auto text : data.items()) {
    texts_.insert({text.key(), text.value()});
  }
}

const std::string& AssetManager::GetText(const std::string& key) const {
  auto it = texts_.find(key);
  if (it == texts_.end()) {
    throw std::runtime_error("Text key not found: " + key);
  }
  return it->second;
}

const std::vector<std::string>& AssetManager::GetMenu(
    const std::string& menu_name) const {
  auto it = menus_.find(menu_name);
  if (it == menus_.end()) {
    throw std::runtime_error("Menu not found: " + menu_name);
  }
  return it->second;
}

// Enemy
std::vector<AssetManager::EnemyData> AssetManager::CreateDefaultEnemies() {
  return enemies_ = {{"Zombie", 15, 5, 2, 20, 10},
                     {"Something in the dark", 20, 7, 3, 25, 15},
                     {"Robot", 25, 8, 5, 30, 20},
                     {"Bandit", 18, 9, 4, 22, 25},
                     {"Wild dog", 12, 6, 1, 15, 8}};
}

AssetManager::EnemyData AssetManager::GetRandomEnemy(int player_level) const {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(0, enemies_.size() - 1);

  EnemyData enemy = enemies_[dist(gen)];

  // Масштабирование характеристик врага
  int level_variation = player_level + (gen() % 3 - 1);
  level_variation = std::max(1, level_variation);

  enemy.base_health = std::max(5, enemy.base_health * level_variation / 2);
  enemy.base_attack = std::max(2, enemy.base_attack * level_variation / 2);
  enemy.base_defense = std::max(1, enemy.base_defense * level_variation / 2);
  enemy.exp_reward *= level_variation;
  enemy.gold_reward *= level_variation;
  enemy.level = level_variation;

  return enemy;
}

void AssetManager::LoadBossData() {
  std::ifstream file("../data/boss_data.json");
  if (!file.is_open()) {
    throw std::runtime_error("Cannot open JSON file: ../data/boss_data.json");
  }
  json data;
  file >> data;
  EnemyData boss;
  for (auto boss_cur : data.items()) {
    boss.name = boss_cur.key();
    boss.base_health = boss_cur.value()[0];
    boss.base_attack = boss_cur.value()[1];
    boss.base_defense = boss_cur.value()[2];
    boss.exp_reward = boss_cur.value()[3];
    boss.gold_reward = boss_cur.value()[4];
    boss_data_ = boss;
  }

  std::string line;
  while (std::getline(file, line)) {
    utils::Trim(line);
    if (line.empty() || line[0] == '#') continue;

    std::istringstream iss(line);
    std::string token;

    if (!std::getline(iss, boss_data_.name, '|')) continue;
    if (!std::getline(iss, token, '|')) continue;
    boss_data_.base_health = std::stoi(token);
    if (!std::getline(iss, token, '|')) continue;
    boss_data_.base_attack = std::stoi(token);
    if (!std::getline(iss, token, '|')) continue;
    boss_data_.base_defense = std::stoi(token);
    if (!std::getline(iss, token, '|')) continue;
    boss_data_.exp_reward = std::stoi(token);
    if (!std::getline(iss, token)) continue;
    boss_data_.gold_reward = std::stoi(token);

    boss_data_.level = 10;  // Уровень босса можно задать фиксированным
    break;                  // Читаем только первую строку
  }

  if (boss_data_.name.empty()) {
    CreateDefaultBoss();
  }
}

void AssetManager::CreateDefaultBoss() {
  boss_data_ = {"MUTATED HUMAN", 150, 20, 10, 500, 1000, 10};
}

AssetManager::EnemyData AssetManager::GetBossData() const { return boss_data_; }
std::map<std::string, AssetManager::CharacterClassData>
AssetManager::GetCharacterClasses() {
  return character_classes_;
}

// Shop
void AssetManager::LoadShopItems() {
  std::ifstream file("../data/shop_items.json");
  if (!file.is_open()) {
    throw std::runtime_error("Cannot open JSON file: ../data/shop_items.json");
  }
  json data;
  file >> data;
  ShopItemData item;
  for (auto& [key, value] : data.items()) {
    item.name = key;
    item.price = value[0];
    item.attack_bonus = value[1];
    item.defense_bonus = value[2];
    item.is_permanent = value[3];
    shop_items_.push_back(item);
  }
}

void AssetManager::CreateDefaultShopItems() {
  shop_items_ = {{"Medical bandages", 20, 0, 0, false},
                 {"Weapon Scope", 100, 2, 0, true},
                 {"Armor Plating", 150, 0, 3, true},
                 {"Armor With Spikes", 120, 1, 1, true}};
}

const std::vector<AssetManager::ShopItemData>& AssetManager::GetShopItems()
    const {
  return shop_items_;
}

}  // namespace rpg