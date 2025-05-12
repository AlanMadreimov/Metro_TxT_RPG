cmake_minimum_required(VERSION 3.30)
project(Trails)

set(CMAKE_CXX_STANDARD 20)

add_executable(Trails Game/Game.cpp
        Game/BattleManager.cpp
        Game/BattleManager.h
        Game/Character.cpp
        Game/Character.h
)
