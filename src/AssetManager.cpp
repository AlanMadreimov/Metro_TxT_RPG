#include "AssetManager.h"
#include "../include/utils.h"
#include <fstream>
#include <sstream>
#include <random>

namespace rpg {

AssetManager::AssetManager() {
    LoadEnemyData();
}

AssetManager::EnemyData AssetManager::GetRandomEnemy(int player_level) const {
    if (enemies_.empty()) {
        return GetDefaultEnemy();
    }

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

AssetManager::EnemyData AssetManager::GetBossData() const {
    return {"MUTATED HUMAN", 100, 15, 8, 300, 500};
}

void AssetManager::LoadEnemyData() {
    std::ifstream file("data/enemies.txt");
    if (!file.is_open()) {
        CreateDefaultEnemies();
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        utils::Trim(line);
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        EnemyData enemy;
        std::string token;

        // Правильный синтаксис для последовательного чтения:
        if (!std::getline(iss, enemy.name, '|')) continue;
        if (!std::getline(iss, token, '|')) continue;
        enemy.base_health = std::stoi(token);
        if (!std::getline(iss, token, '|')) continue;
        enemy.base_attack = std::stoi(token);
        if (!std::getline(iss, token, '|')) continue;
        enemy.base_defense = std::stoi(token);
        if (!std::getline(iss, token, '|')) continue;
        enemy.exp_reward = std::stoi(token);
        if (!std::getline(iss, token)) continue;
        enemy.gold_reward = std::stoi(token);

        enemies_.push_back(enemy);
    }

    if (enemies_.empty()) {
        CreateDefaultEnemies();
    }
}

void AssetManager::CreateDefaultEnemies() {
    enemies_ = {
        {"Zombie", 15, 5, 2, 20, 10},
        {"Something in the dark", 20, 7, 3, 25, 15},
        {"Robot", 25, 8, 5, 30, 20},
        {"Bandit", 18, 9, 4, 22, 25},
        {"Wild dog", 12, 6, 1, 15, 8}
    };
}

AssetManager::EnemyData AssetManager::GetDefaultEnemy() const {
    return {"Unknown Enemy", 15, 5, 2, 20, 10};
}

} // namespace rpg