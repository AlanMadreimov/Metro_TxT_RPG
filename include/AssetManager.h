#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <string>
#include <vector>

namespace rpg {

    class AssetManager {
    public:
        struct EnemyData {
            std::string name;
            int base_health;
            int base_attack;
            int base_defense;
            int exp_reward;
            int gold_reward;
            int level = 1;
        };

        AssetManager();
        EnemyData GetRandomEnemy(int player_level) const;
        EnemyData GetBossData() const;

    private:
        std::vector<EnemyData> enemies_;
        EnemyData boss_data_;

        void LoadEnemyData();
        void LoadBossData();
        void CreateDefaultEnemies();
        void CreateDefaultBoss();
        EnemyData GetDefaultEnemy() const;
    };

} // namespace rpg

#endif // ASSET_MANAGER_H