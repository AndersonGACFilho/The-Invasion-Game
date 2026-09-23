cmake_minimum_required(VERSION 3.16)
project(TheInvasion VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(the_invasion
    src/main.cpp

    src/core/Game.cpp

    src/entities/Entity.cpp
    src/entities/Player.cpp
    src/entities/Enemy.cpp
    src/entities/Projectile.cpp

    src/systems/Starfield.cpp
    src/systems/EnemyFleet.cpp
    src/systems/PlayerWeapon.cpp
    src/systems/EnemyWeapon.cpp
    src/systems/CombatResolver.cpp
    src/systems/Hud.cpp

    src/scenes/MenuScene.cpp
    src/scenes/CreditsScene.cpp
    src/scenes/PlayScene.cpp
    src/scenes/GameOverScene.cpp

    src/platform/Platform.cpp
    src/platform/ResourceLibrary.cpp
    src/platform/AudioDirector.cpp
    src/platform/HighScoreStore.cpp
    src/platform/Paths.cpp
)

# Headers are included as "<folder>/<File>.h", rooted at src/.
target_include_directories(the_invasion PRIVATE src)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(the_invasion PRIVATE -Wall -Wextra -Wpedantic)
endif()

# A system-wide install keeps its assets under a read-only prefix, so the
# game is told where they live and moves its save file to the XDG directory.
# Left unset, the game runs straight from the source tree.
set(THE_INVASION_DATADIR "" CACHE STRING
    "Installed data directory, e.g. /usr/share/the-invasion")

if(THE_INVASION_DATADIR)
    target_compile_definitions(the_invasion PRIVATE
        THE_INVASION_DATADIR="${THE_INVASION_DATADIR}")
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(ALLEGRO REQUIRED IMPORTED_TARGET
    allegro-5
    allegro_main-5
    allegro_image-5
    allegro_font-5
    allegro_ttf-5
    allegro_audio-5
    allegro_acodec-5
    allegro_primitives-5
)
target_link_libraries(the_invasion PRIVATE PkgConfig::ALLEGRO)

if(THE_INVASION_DATADIR)
    include(GNUInstallDirs)
    install(TARGETS the_invasion RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
    install(DIRECTORY Assets fonts sounds DESTINATION "${THE_INVASION_DATADIR}")
else()
    # Running from the source tree: place the binary beside the assets it
    # resolves relative to the working directory.
    set_target_properties(the_invasion PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )
endif()
