cmake_minimum_required(VERSION 3.10)
project(rpg_game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Создаем папку data, если ее нет
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data)

# Копируем файлы данных
configure_file(data/character_data.txt ${CMAKE_BINARY_DIR}/data/character_data.txt COPYONLY)

add_executable(rpg_game
        src/main.cpp
        src/character.cpp
        src/game.cpp
        src/utils.cpp
        include/AssetManager.h
        src/AssetManager.cpp
)

target_include_directories(rpg_game PRIVATE include)