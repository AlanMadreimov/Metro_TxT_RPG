#include <string>
#include <vector>
#include "../Items/Item.h"
#include "../World/Events.h"

#ifndef STATION_H
#define STATION_H

class NPC;

class Station {
private:
    std::string name;
    std::string description;
    std::vector<NPC> npcs;
    std::vector<Item> canFindItems;
    std::vector<Events> canBeEvents;
public:

};



#endif //STATION_H
