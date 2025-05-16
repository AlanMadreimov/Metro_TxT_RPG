#include "../Fight/BattleManager.h"
#include <string>
#include <iostream>

std::atomic<bool> commandEntered(false);
std::mutex mtx;
std::condition_variable cv;

void BattleManager::addParticipant(Character* character) {
    participants.push_back(character);
}

void BattleManager::inputListener() {
    std::string input;
    std::getline(std::cin, input);
    if (input == "heal") {
        commandEntered = true;
        cv.notify_one();
    }
}


void BattleManager::startBattle() {
    std::sort(participants.begin(), participants.end(), 
        [](Character* a, Character* b) { return a->getSpeed() > b->getSpeed(); });

    while (true) {
        for (Character* actor : participants) {
            if (!actor->isAlive()) continue;


            for (Character* target : participants) {
                if (target != actor && target->isAlive()) {
                    actor->attackTarget(*target);
                    break;
                }
            }

            int aliveCount = 0;
            for (Character* c : participants) {
                if (c->isAlive()) aliveCount++;
            }
            if (aliveCount <= 1) {
                std::cout << "Бой окончен!\n";
                return;
            }
        }
    }
}
