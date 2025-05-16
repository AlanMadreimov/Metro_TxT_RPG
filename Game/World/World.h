#include <vector>
#include <string>

#ifndef WORLD_H
#define WORLD_H

class Station;

class World {
private:
    std::vector <Station> stations;

public:
    void moveTo(std::string stationName){};

};



#endif //WORLD_H
