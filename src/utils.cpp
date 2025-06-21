#include "../include/utils.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace rpg {
namespace utils {

void ClearScreen() {
#ifdef _WIN32
  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD coord = {0, 0};
  DWORD count;
  CONSOLE_SCREEN_BUFFER_INFO csbi;

  if (GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
    FillConsoleOutputCharacter(hStdOut, ' ', csbi.dwSize.X * csbi.dwSize.Y,
                               coord, &count);
    SetConsoleCursorPosition(hStdOut, coord);
  }
#else
  std::cout << "\033[2J\033[1;1H";  // ANSI escape codes
#endif
}

void Trim(std::string& str) {
  str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int ch) {
              return !std::isspace(ch);
            }));
  str.erase(std::find_if(str.rbegin(), str.rend(),
                         [](int ch) { return !std::isspace(ch); })
                .base(),
            str.end());
}

}  // namespace utils
}  // namespace rpg