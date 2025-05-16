#include <iostream>
//#include "Fight/BattleManager.h"
#include <fstream>
#include <filesystem>
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

void load () {

}

using namespace std;
int main() {
    string g;
    cin >> g;
    save(g);
    return 0;
}
