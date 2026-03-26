cmake_minimum_required(VERSION 3.16)
project(retro_poker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(external/SFML)


add_executable(retro_poker
    src/aiplayer.cpp
    src/constants.cpp
    src/game.cpp
    src/handevoluator.cpp
    src/logger.cpp
    src/player.cpp
    src/plays.cpp
    src/utils.cpp
    src/gui/gui_main.cpp
)

target_include_directories(retro_poker PRIVATE include)

target_link_libraries(retro_poker PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)

