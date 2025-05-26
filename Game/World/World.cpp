#include <iostream>
#include "World.h"
#include "../Utilities.h"
#include <conio.h>
using namespace std;
void World::start() {
    ClearConsole();
    cout << "You are awake from the long dream\nOh hell... What is going on, where is my wife?\n Press any button...";
    _getch();

}