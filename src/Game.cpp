#include "../include/Game.h"

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <nlohmann/json.hpp>

#include "../include/AssetManager.h"
#include "../include/utils.h"
using json = nlohmann::json;

namespace rpg {

Game::Game() : game_running_(false), save_file_("../data/character_data.txt") {
  std::srand(std::time(nullptr));
  asset_manager_.LoadCharacterClasses();
  asset_manager_.LoadShopItems();
  asset_manager_.LoadEnemyData();
  asset_manager_.LoadBossData();
  asset_manager_.LoadMenuData();
  asset_manager_.LoadTextData();
  asset_manager_.LoadNumericData();
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
    std::cout << asset_manager_.GetText("main_menu_title");
    PrintMenu("Main Menu", asset_manager_.GetMenu("main_menu"));

    int choice = GetChoice(1, asset_manager_.GetMenu("main_menu").size());
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
  ResetCharacter();

  utils::ClearScreen();
  std::cout << asset_manager_.GetText("create_character_title");

  std::string name;
  while (name.empty()) {
    std::cout << "Enter your character's name: ";
    std::getline(std::cin, name);
    utils::Trim(name);
  }

  const auto& character_classes = asset_manager_.GetCharacterClasses();
  std::vector<std::string> class_names;
  for (const auto& [name, _] : character_classes) {
    class_names.push_back(name);
  }

  PrintMenu("Choose your class", class_names);

  int class_choice = GetChoice(1, class_names.size());
  const std::string& chosen_class = class_names[class_choice - 1];
  const auto& class_data = character_classes.at(chosen_class);

  // Устанавливаем основные параметры персонажа
  character_.SetName(name);
  character_.SetClass(chosen_class);
  character_.SetAttack(class_data.base_attack);
  character_.SetDefense(class_data.base_defense);
  character_.SetMaxHp(class_data.base_health);
  character_.AddHealth(class_data.base_health);

  // Добавляем стартовые предметы
  for (const auto& [item, count] : class_data.starting_items) {
    character_.AddItem(item, count);
  }

  // Устанавливаем стартовое золото
  character_.SetGold(class_data.starting_gold);

  game_running_ = true;
  std::cout << "Character created successfully!\n";
  std::cout << "You received starting items and " << class_data.starting_gold
            << " gold!\n";
  WaitForInput();

  // Показываем историю класса
  ShowClassHistory(chosen_class);
}

void Game::ShowClassHistory(const std::string& character_class) {
  utils::ClearScreen();

  if (character_class == "Warrior") {
    std::cout << asset_manager_.GetText("warrior_hist");
  } else if (character_class == "Huckster") {
    std::cout << asset_manager_.GetText("huckster_hist");
  } else if (character_class == "Mechanic") {
    std::cout << asset_manager_.GetText("mechanic_hist");
  } else if (character_class == "Medic") {
    std::cout << asset_manager_.GetText("medic_hist");
  }

  WaitForInput();
}

void Game::LoadCharacter() {
  utils::ClearScreen();
  std::cout << asset_manager_.GetText("load_character_title");

  if (character_.LoadFromFile(save_file_)) {
    std::cout << "Character loaded successfully!\n";
    game_running_ = true;
  } else {
    std::cout << "Failed to load character. Starting new game instead\n";
    CreateCharacter();
  }

  WaitForInput();
}

void Game::GameLoop() {
  while (game_running_ && !game_completed_) {
    utils::ClearScreen();
    std::cout << asset_manager_.GetText("game_location_camp");
    PrintMenu("What will you do?", asset_manager_.GetMenu("game_menu"));

    int choice = GetChoice(1, asset_manager_.GetMenu("game_menu").size());
    switch (choice) {
      case 1:  // Explore
        Explore();
        break;
      case 2:  // Shop
        Shop();
        break;
      case 3:  // Gym
        Gym();
        break;
      case 4:  // Sleep
        Sleep();
        break;
      case 5:  // Status
        ShowStatus();
        break;
      case 6:  // Save Game
        SaveGame();
        break;
      case 7:  // Main Menu
        game_running_ = false;
        break;
    }
  }
  if (game_completed_) {
    game_completed_ = false;
    character_ = Character();  // Сброс персонажа
  }
}

void Game::ShowStatus() const {
  utils::ClearScreen();
  std::cout << asset_manager_.GetText("status_title");
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

void Game::Sleep() {
  std::cout << ("Pay " +
                std::to_string(asset_manager_.GetNum("gold_for_sleep")) +
                " gold or get out of here\n");
  PrintMenu("Sleep Options", asset_manager_.GetMenu("sleep_options"));
  int choice = GetChoice(1, 2);
  if (choice == 1) {
    if (character_.GetGold() >= asset_manager_.GetNum("gold_for_sleep")) {
      character_.AddHealth(character_.GetMaxHealth());
      character_.AddGold(-asset_manager_.GetNum("gold_for_sleep"));
      std::cout << "ZZZ....";
      WaitForInput();
      utils::ClearScreen();
    } else {
      std::cout << "You don't have enough money";
      WaitForInput();
      utils::ClearScreen();
    }
  }
}

void Game::Gym() {
  std::cout << ("You pay with your health for every workout" +
                std::to_string(asset_manager_.GetNum("hp_to_gym")) + "\n");
  PrintMenu("Gym Options", asset_manager_.GetMenu("gym_options"));
  int choice = GetChoice(1, 3);
  if (choice == 1) {
    if (character_.GetHealth() >= asset_manager_.GetNum("hp_to_gym")) {
      character_.AddHealth(-asset_manager_.GetNum("hp_to_gym"));
      character_.SetAttack(character_.GetAttack() +
                           asset_manager_.GetNum("gym_attack_updt"));
      std::cout << "Sounds of a tough workout...\n";
      WaitForInput();
      utils::ClearScreen();
      if (character_.GetHealth() <= 0) {
        std::cout
            << "You overtrained, your health failed, and you had to pay " +
                   std::to_string(asset_manager_.GetNum("gold_for_revieve")) +
                   " gold\n\n";
        if (character_.GetGold() <= asset_manager_.GetNum("gold_for_revieve")) {
          std::cout << "You haven't enought gold to reviev\n";
          Dead();
        } else {
          character_.AddGold(-asset_manager_.GetNum("gold_for_revieve"));
          character_.AddHealth(character_.GetMaxHealth());
        }
        WaitForInput();
      }
      utils::ClearScreen();
    }
  }
  if (choice == 2) {
    if (character_.GetHealth() >= asset_manager_.GetNum("hp_to_gym")) {
      character_.AddHealth(-asset_manager_.GetNum("hp_to_gym"));
      character_.SetDefense(character_.GetDefense() +
                            asset_manager_.GetNum("gym_defense_updt"));
      std::cout << "Sounds of a tough workout...\n";
      WaitForInput();
      utils::ClearScreen();
      if (character_.GetHealth() <= 0) {
        std::cout << "You overtrained, your health failed, and you had to pay" +
                         std::to_string(asset_manager_.GetNum("hp_to_gym")) +
                         " gold\n\n";
        if (character_.GetGold() <= asset_manager_.GetNum("gold_for_revieve")) {
          std::cout << "You haven't enought gold to reviev\n";
          Dead();
        } else {
          character_.AddGold(-asset_manager_.GetNum("gold_for_revieve"));
          character_.AddHealth(character_.GetMaxHealth());
        }
        WaitForInput();
      }
      utils::ClearScreen();
    }
  }
  if (choice == 3) {
    utils::ClearScreen();
  }
}

void Game::Shop() {
  utils::ClearScreen();
  std::cout << asset_manager_.GetText("shop_title");
  std::cout << "Gold: " << character_.GetGold() << "\n\n";

  const auto& shop_items = asset_manager_.GetShopItems();
  std::vector<std::string> options;

  for (const auto& item : shop_items) {
    std::string option = item.name;

    if (item.attack_bonus > 0 || item.defense_bonus > 0) {
      option += " (";
      if (item.attack_bonus > 0) {
        option += "+" + std::to_string(item.attack_bonus) + " Attack";
        if (item.defense_bonus > 0) option += ", ";
      }
      if (item.defense_bonus > 0) {
        option += "+" + std::to_string(item.defense_bonus) + " Defense";
      }
      option += ")";
    }

    option += " - " + std::to_string(item.price) + " gold";
    if (item.is_permanent) option += " (PERMANENT)";

    if (character_.HasItem(item.name)) {
      option += " (PURCHASED)";
    }

    options.push_back(option);
  }
  options.push_back("Exit");

  PrintMenu("Shop Items", options);
  int choice = GetChoice(1, options.size());

  if (choice <= shop_items.size()) {
    const auto& selected_item = shop_items[choice - 1];

    if (character_.HasItem(selected_item.name)) {
      std::cout << "\nYou already have this item!\n";
      WaitForInput();
      return;
    }

    if (character_.GetGold() >= selected_item.price) {
      character_.AddGold(-selected_item.price);

      // Применяем бонусы
      if (selected_item.attack_bonus > 0) {
        character_.AddPermanentAttackBonus(selected_item.attack_bonus);
      }
      if (selected_item.defense_bonus > 0) {
        character_.AddPermanentDefenseBonus(selected_item.defense_bonus);
      }

      character_.AddItem(selected_item.name);

      std::cout << "\nYou bought a " << selected_item.name << "!\n";
      if (selected_item.is_permanent) {
        std::cout << "Your stats have improved permanently!\n";
        std::cout << "Current Attack: " << character_.GetAttack() << "\n";
        std::cout << "Current Defense: " << character_.GetDefense() << "\n";
      }
    } else {
      std::cout << "\nNot enough gold!\n";
    }
    WaitForInput();
  }
}

void Game::ResetCharacter() {
  character_ = Character();
  game_completed_ = false;
  game_running_ = true;
}

//-------------------------------------------------------------------------------------------
void Game::Dead() {
  std::cout << "You haven't enought gold to reviev\n";
  std::cout << "You lose, sucker!!\n";
  WaitForInput();
  game_running_ = false;
}

void Game::Explore() {
  utils::ClearScreen();
  std::cout << asset_manager_.GetText("explore_title");

  if (character_.GetLevel() >= asset_manager_.GetNum("level_to_finish_game") &&
      !game_completed_) {
    std::cout << "\nYou feel something strange... IT'S A MUTATED HUMAN!!! "
                 "What's happend with his arms?! Why is it so huge?!!?1\n";
    WaitForInput();
    FinalBossBattle();
    return;
  }

  // 30% chance встретить врага
  if (std::rand() % 100 < asset_manager_.GetNum("chanche_to_enemy")) {
    auto enemy_data = asset_manager_.GetRandomEnemy(character_.GetLevel());
    std::cout << "You encountered a " << enemy_data.name << "!\n";
    WaitForInput();
    Battle(enemy_data);
  } else {
    // 50% chance найти что-то полезное
    if (std::rand() % 100 < asset_manager_.GetNum("chance_to_spawn_gold")) {
      int gold_found =
          10 + std::rand() % asset_manager_.GetNum("chance_gold_amount");
      character_.AddGold(gold_found);
      std::cout << "You found " << gold_found << " gold!\n";
    } else {
      std::cout << "You explored the area but found nothing of interest.\n";
    }
    WaitForInput();
  }
}

void Game::Battle(const AssetManager::EnemyData& enemy_data) {
  std::string enemy_name = enemy_data.name;
  int enemy_health = enemy_data.base_health;
  int enemy_attack = enemy_data.base_attack;
  int enemy_defense = enemy_data.base_defense;
  int experience_reward = enemy_data.exp_reward;
  int gold_reward = enemy_data.gold_reward;
  int enemy_level = enemy_data.level;

  std::cout << enemy_name << " (Lv " << enemy_level << ") appears!\n";

  while (character_.GetHealth() > 0 && enemy_health > 0) {
    std::cout << "\nYour HP: " << character_.GetHealth() << "/"
              << character_.GetMaxHealth() << "\n";
    std::cout << enemy_name << " HP: " << enemy_health << "\n\n";

    PrintMenu("Battle Options", asset_manager_.GetMenu("battle"));
    int choice = GetChoice(1, asset_manager_.GetMenu("battle").size());

    if (asset_manager_.GetMenu("battle")[choice - 1] == "Attack") {
      // Игрок атакует
      int damage = std::max(1, character_.GetAttack() - enemy_defense / 2);
      enemy_health -= damage;
      std::cout << "You hit the " << enemy_name << " for " << damage
                << " damage!\n";

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
      int enemy_damage =
          std::max(1, enemy_attack - character_.GetDefense() / 2);
      character_.AddHealth(-enemy_damage);
      std::cout << enemy_name << " hits you for " << enemy_damage
                << " damage!\n";

      // Проверяем, не побежден ли игрок
      if (character_.GetHealth() <= 0) {
        std::cout << "You were defeated by the " << enemy_name << "!\n";
        std::cout << "You lost some gold and were returned to town.\n";
        character_.AddGold(-gold_reward / 2);
        if (character_.GetGold() <= 0) {
          Dead();
        }
        character_.AddHealth(character_.GetMaxHealth());  // Восстановление
        break;
      }
    } else if (asset_manager_.GetMenu("battle")[choice - 1] ==
               "Use Medical bandages") {
      if (character_.UseItem("Medical bandages")) {
        int heal_amount = asset_manager_.GetNum("how_many_hp_heal") +
                          character_.GetLevel() * 5;
        character_.AddHealth(heal_amount);
        std::cout << "You used a Medical bandages and recovered " << heal_amount
                  << " HP!\n";
      } else {
        std::cout << "You don't have a Medical bandages\n";
      }
    } else if (asset_manager_.GetMenu("battle")[choice - 1] == "Run Away") {
      if (std::rand() % 100 <
          asset_manager_.GetNum("chance_to_run")) {  // 50% chance убежать
        std::cout << "You successfully ran away!";
        break;
      } else {
        std::cout << "You failed to run away!\n";
        // Враг атакует
        int enemy_damage =
            std::max(1, enemy_attack - character_.GetDefense() / 2);
        character_.AddHealth(-enemy_damage);
        std::cout << enemy_name << " hits you for " << enemy_damage
                  << " damage!\n";

        if (character_.GetHealth() <= 0) {
          std::cout << "You were defeated by the " << enemy_name << "!\n";
          std::cout << "You lost some gold and were returned to town.\n";
          character_.AddGold(-gold_reward / 2);
          if (character_.GetGold() <= 0) {
            Dead();
          }
          character_.AddHealth(character_.GetMaxHealth());  // Восстановление
          break;
        }
      }
    }

    WaitForInput();
    utils::ClearScreen();
  }

  WaitForInput();
}  // Чекнуть на смерть окончательную

void Game::FinalBossBattle() {
  auto boss_data = asset_manager_.GetBossData();
  std::string boss_name = boss_data.name;
  int boss_hp = boss_data.base_health;
  int boss_attack = boss_data.base_attack;
  int boss_defense = boss_data.base_defense;
  int boss_exp = boss_data.exp_reward;
  int boss_gold = boss_data.gold_reward;
  int turn_counter = 0;
  bool boss_enhanced = false;

  std::cout << asset_manager_.GetText("final_title");
  std::cout << boss_name << " appears!\n\n";

  while (character_.GetHealth() > 0 && boss_hp > 0) {
    turn_counter++;

    // Проверка усиления босса
    if (turn_counter % asset_manager_.GetNum("how_often_boss_rage") == 0 &&
        !boss_enhanced) {
      boss_attack += asset_manager_.GetNum("how_often_boss_rage");
      boss_enhanced = true;
      std::cout << boss_name << " getting strong! His attack is now "
                << boss_attack << "!\n";
    }

    // Стандартный бой
    int damage = std::max(1, character_.GetAttack() - boss_defense / 2);
    boss_hp -= damage;
    std::cout << "You hit " << damage << " damage!\n";

    if (boss_hp <= 0) {
      std::cout << "\nYou won " << boss_name << "!!!\n";
      character_.AddGold(boss_gold);
      character_.AddExperience(boss_exp);
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
      character_.AddGold(-boss_gold / 10);  // Штраф за смерть
      if (character_.GetGold() <= 0) {
        Dead();
      }
      return;
    }

    WaitForInput();
  }
}  // смерть игрока

void Game::ShowEnding() {
  utils::ClearScreen();
  std::cout << asset_manager_.GetText("ending_hist");
  std::cout << "Pay for a full version\n";
  WaitForInput();
  utils::ClearScreen();
  std::cout << "=== WIN ===\n\n";
  std::cout
      << "You won the mutated human and finish your adventure for now!\n\n";

  std::cout << "Gamer statistic:\n";
  std::cout << "Level: " << character_.GetLevel() << "\n";
  std::cout << "Summary gold: " << character_.GetGold() << "\n";

  std::cout << "Thanks for the game!\n";
  WaitForInput();
  game_running_ = false;
}

bool Character::HasItem(const std::string& item) const {
  return std::find(purchased_upgrades_.begin(), purchased_upgrades_.end(),
                   item) != purchased_upgrades_.end();
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
      std::cout << "Invalid input. Please enter a number between " << min
                << " and " << max << ": ";
    } else {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      return choice;
    }
  }
}

void Game::WaitForInput() const {
  std::cout << asset_manager_.GetText("continue");
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

}  // namespace rpg