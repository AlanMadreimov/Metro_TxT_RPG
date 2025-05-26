#include <string>
#include <vector>
#include "../Items/Item.h"
#include "../World/Events.h"
#include "../World/NPC.h"

#ifndef STATION_H
#define STATION_H



class Station {
private:
    std::string name;
    std::string description;
    std::vector<NPC> npcs;
    std::vector<Item> canFindItems;
    std::vector<Events> canBeEvents;
public:
    void inspect(){};
    void search(){};
    void talkTo(){};
};



#endif //STATION_H
