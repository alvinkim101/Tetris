cmake_minimum_required(VERSION 3.22)
project(Tetris LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Tetris
add_executable(Tetris src/main.cpp)

target_include_directories(Tetris PRIVATE src)
target_sources(Tetris PRIVATE
    src/Tetris.cpp
    src/Board.cpp
    src/Tetromino.cpp
)

# SDL2
target_link_libraries(Tetris PRIVATE SDL2)
target_link_libraries(Tetris PRIVATE SDL2_mixer)
