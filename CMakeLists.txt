cmake_minimum_required(VERSION 3.30)
project(Trails)

set(CMAKE_CXX_STANDARD 20)

add_executable(Trails Game/Game.cpp
        Game/Fight/BattleManager.cpp
        Game/Fight/BattleManager.h
        Game/Characters/Character.cpp
        Game/Characters/Character.h
        Game/World/World.cpp
        Game/World/World.h
        Game/World/Station.cpp
        Game/World/Station.h
        Game/Utilities.cpp
        Game/Utilities.h
)
