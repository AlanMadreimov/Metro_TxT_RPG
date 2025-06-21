cmake_minimum_required(VERSION 3.10)
project(rpg_game)
include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB MY_SOURCES "src/*.cpp")
add_executable(rpg_game ${MY_SOURCES})


FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

target_include_directories(rpg_game PRIVATE include)
target_link_libraries(rpg_game PRIVATE nlohmann_json::nlohmann_json)