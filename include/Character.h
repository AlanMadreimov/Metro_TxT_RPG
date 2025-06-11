#ifndef RPG_GAME_CHARACTER_H_
#define RPG_GAME_CHARACTER_H_

#include <string>
#include <map>

namespace rpg {

    class Character {
    public:
        Character();
        explicit Character(const std::string& file_path);

        // Загрузка и сохранение персонажа
        bool LoadFromFile(const std::string& file_path);
        bool SaveToFile(const std::string& file_path) const;

        // Основные характеристики
        int GetLevel() const { return level_; }
        int GetHealth() const { return health_; }
        int GetMaxHealth() const { return max_health_; }
        int GetAttack() const { return attack_; }
        int GetDefense() const { return defense_; }
        int GetGold() const { return gold_; }
        int GetExperience() const { return experience_; }
        const std::string& GetName() const { return name_; }
        const std::string& GetClass() const { return class_; }

        // Модификаторы характеристик
        void SetName(const std::string& name) { name_ = name; }
        void SetClass(const std::string& character_class) { class_ = character_class; }
        void AddHealth(int value);
        void AddGold(int value) { gold_ += value; }
        void AddExperience(int value);
        void LevelUp();

        // Инвентарь
        void AddItem(const std::string& item, int quantity = 1);
        bool UseItem(const std::string& item);
        int GetItemCount(const std::string& item) const;
        const std::map<std::string, int>& GetInventory() const { return inventory_; }

    private:
        std::string name_;
        std::string class_;
        int level_;
        int health_;
        int max_health_;
        int attack_;
        int defense_;
        int gold_;
        int experience_;
        int experience_to_next_level_;
        std::map<std::string, int> inventory_;

        void CalculateStats();
    };

}  // namespace rpg

#endif  // RPG_GAME_CHARACTER_H_