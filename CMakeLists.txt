cmake_minimum_required(VERSION 3.24)

project(SnakeGame)

include(FetchContent)


FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/4.2.0.zip
)

FetchContent_MakeAvailable(raylib)

add_executable(SnakeGame main.c)

target_link_libraries(SnakeGame raylib)

