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
)

target_include_directories(core PUBLIC src)

# ----ui----
add_library(ui
    src/ui/boardRender.cpp
)

target_link_libraries(ui PUBLIC core sfml-graphics sfml-window sfml-system)


target_include_directories(core PUBLIC src)
target_include_directories(ui PUBLIC src)


add_executable(ChessClash
    src/main.cpp)

add_executable(TestFile
    src/tester.cpp)

target_compile_features(ChessClash PRIVATE cxx_std_17)
target_link_libraries(ChessClash
    PRIVATE
        ui
        core
        sfml-graphics
        sfml-window
        sfml-system
)
