#include <iostream>
#include "BattleManager.h"
using namespace std;

std::atomic<bool> commandEntered(false);
std::mutex mtx;
std::condition_variable cv;

int main() {
    cout << "This Is Pizdec";

    return 0;
}
