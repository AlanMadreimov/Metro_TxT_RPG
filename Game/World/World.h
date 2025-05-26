#include <vector>
#include <string>
#include "Station.h"
#ifndef WORLD_H
#define WORLD_H

class World {
private:
    std::vector <Station> stations;
    Station currentStation;
    int currentWorldTime;
public:
    World() {}
    World(std::vector <Station> stations_sv) {}

    void moveTo(std::string stationName){};
    void goWorldTime(){};
    int getWorldTime(){};
    void start(){};
};



#endif //WORLD_H
