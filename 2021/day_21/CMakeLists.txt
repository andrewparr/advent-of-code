cmake_minimum_required(VERSION 3.0)
project(aoc)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS "program_options")
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

set(CMAKE_CXX_STANDARD 20)
add_executable(day_21 day_21.cpp)
target_link_libraries(day_21 gmp)
