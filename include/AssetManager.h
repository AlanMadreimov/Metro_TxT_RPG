#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <map>
#include <string>
#include <vector>

namespace rpg {

class AssetManager {
 public:
  struct ShopItemData {
    std::string name;
    int price;
    int attack_bonus;
    int defense_bonus;
    bool is_permanent;
  };
  struct EnemyData {
    std::string name;
    int base_health;
    int base_attack;
    int base_defense;
    int exp_reward;
    int gold_reward;
    int level = 1;
  };
  struct CharacterClassData {
    int base_attack;
    int base_defense;
    int base_health;
    int starting_gold;
    std::map<std::string, int> starting_items;
  };
  AssetManager();

  // Enemy
  EnemyData GetRandomEnemy(int player_level) const;
  EnemyData GetBossData() const;
  void LoadEnemyData();
  void LoadBossData();

  // Shop
  const std::vector<ShopItemData>& GetShopItems() const;
  void LoadShopItems();

  // Text
  void LoadTextData();
  void LoadMenuData();
  const std::string& GetText(const std::string& key) const;
  const std::vector<std::string>& GetMenu(const std::string& menu_name) const;

  // Other
  void LoadCharacterClasses();
  std::map<std::string, CharacterClassData> GetCharacterClasses();
  void LoadNumericData();
  int GetNum(const std::string& key);

 private:
  // Other
  std::map<std::string, CharacterClassData> character_classes_;
  // Menu
  std::map<std::string, std::string> texts_;
  std::map<std::string, std::vector<std::string>> menus_;
  std::map<std::string, int> numeric_;

  // Shop
  std::vector<ShopItemData> shop_items_;
  void CreateDefaultShopItems();

  // Enemy
  std::vector<EnemyData> enemies_;
  EnemyData boss_data_;
  std::vector<EnemyData> CreateDefaultEnemies();
  void CreateDefaultBoss();
};

}  // namespace rpg

#endif  // ASSET_MANAGER_H