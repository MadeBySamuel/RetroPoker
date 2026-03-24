cmake_minimum_required(VERSION 3.16)
project(retro_poker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(retro_poker
    aiplayer.cpp
    constants.cpp
    game.cpp
    handevoluator.cpp
    logger.cpp
    player.cpp
    plays.cpp
    utils.cpp
)
