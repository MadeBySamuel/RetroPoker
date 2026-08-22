cmake_minimum_required(VERSION 3.16)
project(retro_poker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(external/SFML)
set(TGUI_BACKEND SFML_GRAPHICS)
add_subdirectory(external/TGUI)

# Keep third-party implementation details out of the debugger. Our own target
# still builds with Debug symbols, so breakpoints and local variables work.
foreach(third_party_target
    sfml-system
    sfml-window
    sfml-graphics
    sfml-audio
    sfml-network
    tgui
)
    target_compile_options(${third_party_target} PRIVATE $<$<CONFIG:Debug>:-g0>)
endforeach()

find_package(SQLite3 REQUIRED)

add_executable(retro_poker
    src/aiplayer.cpp
    src/constants.cpp
    src/game.cpp
    src/hand_evaluator.cpp
    src/logger.cpp
    src/player.cpp
    src/plays.cpp
    src/utils.cpp
    src/gui/gui_login.cpp
    src/gui/gui_main_menu.cpp
    src/gui/gui_game.cpp
    src/gui/main.cpp
    src/auth.cpp
    src/gui/gui_utils.cpp
)

add_custom_target(run
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target retro_poker
    COMMAND ${CMAKE_BINARY_DIR}/retro_poker
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS retro_poker
    USES_TERMINAL
    COMMENT "Build and run RetroPoker"
)



add_library(argon2 STATIC
    external/argon2-hashing/src/argon2.c
    external/argon2-hashing/src/core.c
    external/argon2-hashing/src/encoding.c
    external/argon2-hashing/src/thread.c
    external/argon2-hashing/src/ref.c
    external/argon2-hashing/src/blake2/blake2b.c
)

target_include_directories(argon2 PUBLIC
    external/argon2-hashing/include
    external/argon2-hashing/src
)

target_include_directories(retro_poker PRIVATE
    include
    include/gui
    external/argon2-hashing/include
    src/gui
    src
)


target_link_libraries(retro_poker PRIVATE
    TGUI::TGUI
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
    argon2
    SQLite3::SQLite3
)

target_compile_options(retro_poker PRIVATE
    $<$<CONFIG:Debug>:-g -O0>
    $<$<CONFIG:Release>:-O2>
)
