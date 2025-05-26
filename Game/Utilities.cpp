#ifdef _WIN32
#include <windows.h>
#endif
#include "Utilities.h"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

void ClearConsole() {
    system("cls");
}
bool searchStringInFile(const string& filename, const string& searchString) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка: не удалось открыть файл '" << filename << "'" << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string word;
        while (iss >> word) {  // Разбиваем строку на слова по пробелам
            if (word == searchString) {
                file.close();
                return true;
            }
        }
    }

    file.close();
    return false;
}