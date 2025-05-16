#ifndef BATTLEMANAGER_H
#define BATTLEMANAGER_H

#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>
#include <vector>
#include <mutex>
#include <condition_variable>
#include "../Characters/Character.h"

class BattleManager {
private:
    std::vector<Character*> participants;



public:
    void addParticipant(Character* character);
    void startBattle();
    void inputListener();
};



#endif //BATTLEMANAGER_H
