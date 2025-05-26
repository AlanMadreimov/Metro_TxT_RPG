#include <iostream>
#include "Fight/BattleManager.h"
#include <fstream>
#include <filesystem>
#include "Utilities.h"
#include "World/World.h"
using namespace std;

class MainMenu {
private:
    string currentSaveName;



    bool save(std::string name) {
        std::filesystem::path dir_path = "C:/TrailsSaves";

        if (!std::filesystem::exists(dir_path)) {
            std::filesystem::create_directory(dir_path);
            std::cout << "Created: " << dir_path << std::endl;
        }

        std::filesystem::path file_path = dir_path / (name + ".txt");
        ofstream file(file_path, std::ios::out);
        if (!file.is_open()) {
            file.open(file_path, std::ios::out);
        }
        file.close();
        fstream savesNames("../SavesNames.txt",fstream::app);
        if (!savesNames.is_open()) {

        }
        if (!searchStringInFile("../SavesNames.txt",name)) {
            savesNames << name + " ";
            savesNames.close();
            return true;
        }
        savesNames.close();
        return false;
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
                ClearConsole();
                cout << "Please enter game name:\n";
                string nameOfSave;
                cin >> nameOfSave;
                if (save(nameOfSave)) {
                    World wrld;
                    wrld.start();
                }
                else {

                }

            } else if (choice == "2") {
                ifstream file("../SavesNames.txt");


                string choice;
                cout << "Please choise";
                cin >> choice;


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
