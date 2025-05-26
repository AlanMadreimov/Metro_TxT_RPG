#include <iostream>
#include "Station.h"
using namespace std;

Station::Station() {
    name = "Station 1";
    description = "Zabroshennaya station of metro";
    cin >> name;
}

void Station::enter() {
    cout << name + "\n";
    cout << description;
}