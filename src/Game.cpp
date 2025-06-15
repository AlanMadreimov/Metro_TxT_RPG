#include "../include/Game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "../include/utils.h"
#include <limits>

namespace rpg {

namespace {

const std::vector<std::string> kMainMenuOptions = {
  "New Game",
  "Load Game",
  "Exit"
};

const std::vector<std::string> kGameMenuOptions = {
  "Explore",
  "Shop",
  "Status",
  "Save Game",
  "Main Menu"
};

const std::vector<std::string> kCharacterClasses = {
  "Warrior",
  "Huckster",
  "Mechanic",
  "Medic"
};

const std::vector<std::string> kEnemyNames = {
  "Goblin",
  "Orc",
  "Skeleton",
  "Bandit",
  "Wolf"
};

const std::vector<std::string> kShopItems = {
  "Health Potion",
  "Attack Boost",
  "Defense Boost"
};

}  // namespace

Game::Game() : game_running_(false), save_file_("data/character_data.txt.txt") {
  std::srand(std::time(nullptr));
}

void Game::Start() {
  while (true) {
    MainMenu();
    if (!game_running_) break;
    GameLoop();
  }
}

void Game::MainMenu() {
  while (true) {
    utils::ClearScreen();
    std::cout << "=== RPG Game ===\n\n";
    PrintMenu("Main Menu", kMainMenuOptions);

    int choice = GetChoice(1, kMainMenuOptions.size());
    switch (choice) {
      case 1:  // New Game
        CreateCharacter();
        return;
      case 2:  // Load Game
        LoadCharacter();
        return;
      case 3:  // Exit
        game_running_ = false;
        return;
    }
  }
}

void Game::CreateCharacter() {
  utils::ClearScreen();
  std::cout << "=== Create Character ===\n\n";

  std::string name;
  while (name.empty()) {
    std::cout << "Enter your character's name: ";
    std::getline(std::cin, name);
    utils::Trim(name);
  }

  std::cout << "\nChoose your class:\n";
  PrintMenu("Classes", kCharacterClasses);

  int class_choice = GetChoice(1, kCharacterClasses.size());
  character_.SetName(name);
  character_.SetClass(kCharacterClasses[class_choice - 1]);

  game_running_ = true;
  std::cout << "\nCharacter created successfully!\n";
  WaitForInput();
}

void Game::LoadCharacter() {
  utils::ClearScreen();
  std::cout << "=== Load Character ===\n\n";

  if (character_.LoadFromFile(save_file_)) {
    std::cout << "Character loaded successfully!\n";
    game_running_ = true;
  } else {
    std::cout << "Failed to load character. Starting new game instead.\n";
    CreateCharacter();
  }

  WaitForInput();
}

void Game::GameLoop() {
  while (game_running_ && !game_completed_) {
    utils::ClearScreen();
    std::cout << "=== Adventure ===\n\n";
    PrintMenu("What will you do?", kGameMenuOptions);

    int choice = GetChoice(1, kGameMenuOptions.size());
    switch (choice) {
      case 1:  // Explore
        Explore();
        break;
      case 2:  // Shop
        Shop();
        break;
      case 3:  // Status
        ShowStatus();
        break;
      case 4:  // Save Game
        SaveGame();
        break;
      case 5:  // Main Menu
        game_running_ = false;
        break;
    }
  }
  if (game_completed_) {
    game_completed_ = false;
    character_ = Character(); // Сброс персонажа
  }
}

void Game::ShowStatus() const {
  utils::ClearScreen();
  std::cout << "=== Character Status ===\n\n";
  std::cout << "Name: " << character_.GetName() << "\n";
  std::cout << "Class: " << character_.GetClass() << "\n";
  std::cout << "Level: " << character_.GetLevel() << "\n";
  std::cout << "Health: " << character_.GetHealth() << "/"
            << character_.GetMaxHealth() << "\n";
  std::cout << "Attack: " << character_.GetAttack() << "\n";
  std::cout << "Defense: " << character_.GetDefense() << "\n";
  std::cout << "Gold: " << character_.GetGold() << "\n";
  std::cout << "Experience: " << character_.GetExperience() << "/"
            << (100 * character_.GetLevel()) << "\n";

  std::cout << "\nInventory:\n";
  const auto& inventory = character_.GetInventory();
  if (inventory.empty()) {
    std::cout << "  (empty)\n";
  } else {
    for (const auto& item : inventory) {
      std::cout << "  " << item.first << ": " << item.second << "\n";
    }
  }

  WaitForInput();
}

void Game::Explore() {
  utils::ClearScreen();
  std::cout << "=== Exploring ===\n\n";

  if (character_.GetLevel() >= 5 && !game_completed_) {
    std::cout << "\nyou feel something starnge... IT'S A DRAGON!!!\n";
    WaitForInput();
    FinalBossBattle();
    return;
  }

  // 30% chance встретить врага
  if (std::rand() % 100 < 30) {
    std::cout << "You encountered an enemy!\n";
    WaitForInput();
    Battle();
  } else {
    // 50% chance найти что-то полезное
    if (std::rand() % 100 < 50) {
      int gold_found = 10 + std::rand() % 20;
      character_.AddGold(gold_found);
      std::cout << "You found " << gold_found << " gold!\n";
    } else {
      std::cout << "You explored the area but found nothing of interest.\n";
    }
    WaitForInput();
  }
}

void Game::Battle() {
  // Создаем врага
  std::string enemy_name = kEnemyNames[std::rand() % kEnemyNames.size()];
  int enemy_level = std::max(1, character_.GetLevel() + (std::rand() % 3 - 1));
  int enemy_health = 15 + enemy_level * 5;
  int enemy_attack = 3 + enemy_level * 2;
  int enemy_defense = 1 + enemy_level;
  int experience_reward = 20 * enemy_level;
  int gold_reward = 5 * enemy_level;

  std::cout << enemy_name << " (Lv " << enemy_level << ") appears!\n";

  while (character_.GetHealth() > 0 && enemy_health > 0) {
    std::cout << "\nYour HP: " << character_.GetHealth() << "/"
              << character_.GetMaxHealth() << "\n";
    std::cout << enemy_name << " HP: " << enemy_health << "\n\n";

    std::vector<std::string> options = {"Attack"};
    if (character_.GetItemCount("Health Potion") > 0) {
      options.push_back("Use Health Potion");
    }
    options.push_back("Run Away");

    PrintMenu("Battle Options", options);
    int choice = GetChoice(1, options.size());

    if (options[choice - 1] == "Attack") {
      // Игрок атакует
      int damage = std::max(1, character_.GetAttack() - enemy_defense / 2);
      enemy_health -= damage;
      std::cout << "You hit the " << enemy_name << " for " << damage << " damage!\n";

      // Проверяем, не побежден ли враг
      if (enemy_health <= 0) {
        std::cout << "You defeated the " << enemy_name << "!\n";
        character_.AddExperience(experience_reward);
        character_.AddGold(gold_reward);
        std::cout << "Gained " << experience_reward << " EXP and "
                  << gold_reward << " gold!\n";
        break;
      }

      // Враг атакует
      int enemy_damage = std::max(1, enemy_attack - character_.GetDefense() / 2);
      character_.AddHealth(-enemy_damage);
      std::cout << enemy_name << " hits you for " << enemy_damage << " damage!\n";

      // Проверяем, не побежден ли игрок
      if (character_.GetHealth() <= 0) {
        std::cout << "You were defeated by the " << enemy_name << "!\n";
        std::cout << "You lost some gold and were returned to town.\n";
        character_.AddGold(-gold_reward / 2);
        character_.AddHealth(character_.GetMaxHealth());  // Восстановление
        break;
      }
    } else if (options[choice - 1] == "Use Health Potion") {
      character_.UseItem("Health Potion");
      int heal_amount = 20 + character_.GetLevel() * 5;
      character_.AddHealth(heal_amount);
      std::cout << "You used a Health Potion and recovered "
                << heal_amount << " HP!\n";
    } else if (options[choice - 1] == "Run Away") {
      if (std::rand() % 100 < 50) {  // 50% chance убежать
        std::cout << "You successfully ran away!\n";
        break;
      } else {
        std::cout << "You failed to run away!\n";
        // Враг атакует
        int enemy_damage = std::max(1, enemy_attack - character_.GetDefense() / 2);
        character_.AddHealth(-enemy_damage);
        std::cout << enemy_name << " hits you for " << enemy_damage << " damage!\n";

        if (character_.GetHealth() <= 0) {
          std::cout << "You were defeated by the " << enemy_name << "!\n";
          std::cout << "You lost some gold and were returned to town.\n";
          character_.AddGold(-gold_reward / 2);
          character_.AddHealth(character_.GetMaxHealth());  // Восстановление
          break;
        }
      }
    }

    WaitForInput();
  }

  WaitForInput();
}

void Game::FinalBossBattle() {
    std::string boss_name = "DEATH DRAGON";
    int boss_hp = 100;
    int boss_attack = 15;
    int boss_defense = 8;
    int turn_counter = 0;
    bool boss_enhanced = false;

    std::cout << "=== FINAL BATTLE ===\n";
    std::cout << boss_name << " appers!\n\n";

    while (character_.GetHealth() > 0 && boss_hp > 0) {
        turn_counter++;

        // Проверка усиления босса
        if (turn_counter % 3 == 0 && !boss_enhanced) {
            boss_attack += 3;
            boss_enhanced = true;
            std::cout << boss_name << " getting strong! His attack is now " << boss_attack << "!\n";
        }

        // Стандартный бой
        int damage = std::max(1, character_.GetAttack() - boss_defense / 2);
        boss_hp -= damage;
        std::cout << "You hit " << damage << " damage!\n";

        if (boss_hp <= 0) {
            std::cout << "\nYou won " << boss_name << "!!!\n";
            character_.AddGold(500);
            character_.AddExperience(300);
            game_completed_ = true;
            ShowEnding();
            return;
        }

        // Критический удар босса
        bool is_critical = (std::rand() % 100) < 20;
        int boss_damage = std::max(1, boss_attack - character_.GetDefense() / 2);

        if (is_critical) {
            boss_damage = static_cast<int>(boss_damage * 1.5);
            std::cout << boss_name << " hit critical damage! ";
        }

        character_.AddHealth(-boss_damage);
        std::cout << boss_name << " attacked " << boss_damage << " damage.\n";

        if (character_.GetHealth() <= 0) {
            std::cout << "You lost against " << boss_name << "...\n";
            character_.AddHealth(character_.GetMaxHealth());
            character_.AddGold(-100); // Штраф за смерть
            return;
        }

        WaitForInput();
    }
}

void Game::ShowEnding() {
    utils::ClearScreen();
    std::cout << "=== WIN ===\n\n";
    std::cout << "You won death dragon and finish your adventure!\n\n";

    std::cout << "Gamer statistic:\n";
    std::cout << "Level: " << character_.GetLevel() << "\n";
    std::cout << "Summary gold: " << character_.GetGold() << "\n";

    std::cout << "Thanks for the game!\n";
    WaitForInput();
    game_running_ = false;
}

void Game::Shop() {
  utils::ClearScreen();
  std::cout << "=== Shop ===\n\n";
  std::cout << "Gold: " << character_.GetGold() << "\n\n";

  std::vector<std::pair<std::string, int>> shop_items = {
    {"Health Potion", 20},
    {"Attack Boost (+2 for current battle)", 30},
    {"Defense Boost (+2 for current battle)", 30}
  };

  std::vector<std::string> options;
  for (const auto& item : shop_items) {
    options.push_back(item.first + " - " + std::to_string(item.second) + " gold");
  }
  options.push_back("Exit");

  PrintMenu("Shop Items", options);
  int choice = GetChoice(1, options.size());

  if (choice <= shop_items.size()) {
    const auto& selected_item = shop_items[choice - 1];
    if (character_.GetGold() >= selected_item.second) {
      character_.AddGold(-selected_item.second);
      character_.AddItem(selected_item.first);
      std::cout << "\nYou bought a " << selected_item.first << "!\n";
    } else {
      std::cout << "\nNot enough gold!\n";
    }
    WaitForInput();
  }
}

void Game::SaveGame() {
  if (character_.SaveToFile(save_file_)) {
    std::cout << "Game saved successfully!\n";
  } else {
    std::cout << "Failed to save game!\n";
  }
  WaitForInput();
}

void Game::PrintMenu(const std::string& title,
                    const std::vector<std::string>& options) const {
  std::cout << title << ":\n";
  for (size_t i = 0; i < options.size(); ++i) {
    std::cout << "  " << (i + 1) << ". " << options[i] << "\n";
  }
  std::cout << "\nEnter your choice: ";
}

int Game::GetChoice(int min, int max) const {
  int choice;
  while (true) {
    std::cin >> choice;
    if (std::cin.fail() || choice < min || choice > max) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Invalid input. Please enter a number between "
                << min << " and " << max << ": ";
    } else {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      return choice;
    }
  }
}

void Game::WaitForInput() const {
  std::cout << "\nPress Enter to continue...";
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

}  // namespace rpg