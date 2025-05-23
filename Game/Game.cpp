#include <iostream>
#include "Fight/BattleManager.h"
#include <fstream>
#include <filesystem>
#include "Utilities.h"
using namespace std;

void save(std::string name) {
    std::filesystem::path dir_path = "C:/TrailsSaves";

    if (!std::filesystem::exists(dir_path)) {
        std::filesystem::create_directory(dir_path); // Или create_directories для вложенных путей
        std::cout << "Created: " << dir_path << std::endl;
    }

    std::filesystem::path file_path = dir_path / (name + ".txt");
    ofstream file(file_path, std::ios::out);
    if (!file.is_open()) {
        file.open(file_path, std::ios::out);
    }
    file.close();
    std::cout << "Saved";

}

void load (std::string name) {
    std::filesystem::path dir_path = "C:/TrailsSaves";
    ifstream file(dir_path / (name + ".txt"));
    if (!file.is_open()) {
        std::cout << "Failed to load file";
    }
    std::cout << "Yeees";
}

class MainMenu {
private:
    string currentSaveName;
    void save(std::string name) {
        std::filesystem::path dir_path = "C:/TrailsSaves";

        if (!std::filesystem::exists(dir_path)) {
            std::filesystem::create_directory(dir_path); // Или create_directories для вложенных путей
            std::cout << "Created: " << dir_path << std::endl;
        }

        std::filesystem::path file_path = dir_path / (name + ".txt");
        ofstream file(file_path, std::ios::out);
        if (!file.is_open()) {
            file.open(file_path, std::ios::out);
        }
        file.close();
        std::cout << "Saved";

    }

    void load (std::string name) {
        std::filesystem::path dir_path = "C:/TrailsSaves";
        ifstream file(dir_path / (name + ".txt"));
        if (!file.is_open()) {
            std::cout << "Failed to load file";
        }
        std::cout << "Yeees";
    }

public:
    void start() {
        while (true) {
            ClearConsole();
            cout << "Menu:\n1. New Game\n2. Load save\n3. Exit\n";
            string choice;
            cin >> choice;
            if (choice == "1") {
                //Give name to save
                //Create World and other thing
                cout << "Starting your engine";
                break;
            }else if (choice == "2") {
                // Menu to choice saves
                // Load and create World with this data
                cout << "Load Screen";
                break;
            }else if (choice == "3"){
                cout << "Exit";
                break;
            }
        }
    }

};

using namespace std;
int main() {
    MainMenu menu;
    menu.start();
    return 0;
}
