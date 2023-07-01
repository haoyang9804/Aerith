cmake_minimum_required(VERSION 3.13.4)
project(Aerith)

find_package(Clang REQUIRED)
set(Clang_ROOT ${CLANG_CMAKE_DIR})

set(CMAKE_CXX_STANDARD 17 CACHE STRING "")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall\
    -fdiagnostics-color=always")

add_library(play SHARED play.cpp)

target_link_libraries(play
  "$<$<PLATFORM_ID:Darwin>:-undefined dynamic_lookup>")