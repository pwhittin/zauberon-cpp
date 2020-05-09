cmake_minimum_required(VERSION 3.16)
project(zauberon)
set(CMAKE_CXX_STANDARD 17)
add_subdirectory(catch-tests)

add_executable(zauberon
        functional-utils.h   functional-utils.cpp
        schneider.h          schnieder.cpp
        main.cpp global-types.h)

