#ifndef RPG_GAME_GAME_H_
#define RPG_GAME_GAME_H_

#include <string>
#include <vector>
#include "AssetManager.h"
#include "Character.h"

namespace rpg {

    class Game {
    public:
        Game();
        void Sleep();
        void Gym();
        void Start();
        void MainMenu();
        void CreateCharacter();
        void LoadCharacter();
        void GameLoop();
        void ShowStatus() const;
        void Explore();
        void Battle(const AssetManager::EnemyData& enemy_data);
        void Shop();
        void SaveGame();

    private:
        AssetManager asset_manager_;
        Character character_;
        bool game_running_;
        std::string save_file_;
        bool game_completed_ = false;
        void PrintMenu(const std::string& title, const std::vector<std::string>& options) const;
        int GetChoice(int min, int max) const;
        void WaitForInput() const;
        void FinalBossBattle();
        void ShowEnding();
    };

}  // namespace rpg

#endif  // RPG_GAME_GAME_H_