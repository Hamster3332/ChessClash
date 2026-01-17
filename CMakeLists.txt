cmake_minimum_required(VERSION 3.28)

project(ChessClash)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(ChessClash)

target_sources(ChessClash
    PRIVATE
        src/main.cpp
)

target_link_libraries(ChessClash
    PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
        core
        ui
        engines
)

add_library(core)

target_sources(core
    PRIVATE
        src/core/board.cpp
        src/core/chessVector.cpp
        src/core/logger.cpp
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            src/core
        FILES
            src/core/board.h
            src/core/chessVector.h
            src/core/logger.h
)

add_library(ui)

target_sources(ui
    PRIVATE
        src/ui/renderBoard.cpp
        src/ui/textureManager.cpp
        src/ui/soundManager.cpp
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            src/ui
        FILES
            src/ui/renderBoard.h
            src/ui/textureManager.h
            src/ui/soundManager.h
)

target_link_libraries(ui
    PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
        core
)

add_library(engines)

target_sources(engines
    PRIVATE
        src/engines/onlinePlayer.cpp
        src/engines/player.cpp
        src/engines/general.cpp
        src/engines/subGeneral.cpp
        src/engines/recursive.cpp
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            src/engines
        FILES
            src/engines/player.h
            src/engines/onlinePlayer.h
            src/engines/general.h
            src/engines/subGeneral.h
            src/engines/recursive.h
)

target_link_libraries(engines
    PRIVATE
        core
        ui
)


#target_compile_definitions(core
#    PRIVATE
#        DEBUG
#)


# ----tester.cpp----
add_executable(tester)
target_sources(tester
    PRIVATE
        src/tester.cpp
)

target_link_libraries(tester
    PRIVATE
        core
        ui
        engines
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
)
