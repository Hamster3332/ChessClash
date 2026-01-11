cmake_minimum_required(VERSION 3.28)
project(ChessClash LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)


# ----core----
add_library(core
    src/core/board.cpp
    src/core/chessVector.cpp
    src/core/logger.cpp
)

target_include_directories(core PUBLIC src)

# ----ui----
add_library(ui
    src/ui/boardRender.cpp
    src/ui/textureManager.cpp
)

target_link_libraries(ui PUBLIC core sfml-graphics sfml-window sfml-system)

# ----engines----
add_library(player
    src/engines/player.cpp
    src/engines/onlinePlayer.cpp
)

target_link_libraries(player PUBLIC core)

target_include_directories(core PUBLIC src)
target_include_directories(ui PUBLIC src)
target_include_directories(player PUBLIC src)


add_executable(ChessClash
    src/main.cpp)

add_executable(TestFile
    src/tester.cpp)

target_compile_features(ChessClash PRIVATE cxx_std_17)
target_link_libraries(ChessClash
    PRIVATE
        ui
        core
        player
        sfml-graphics
        sfml-window
        sfml-system
)
