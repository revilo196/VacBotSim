cmake_minimum_required(VERSION 3.12)
project(VacBotSim)

set(CMAKE_CXX_STANDARD 11)

find_library(SIMPLE2D simple2d)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(C:\\Users\\Oliver\\AppData\\Local\\simple2d\\include)

add_subdirectory(./Box2D)
add_subdirectory(./VacEnv)
add_subdirectory(./MultiNEAT)


include_directories(${Boost_INCLUDE_DIRS})
include_directories( "C:\\local\\boost_1_69_0")

add_executable(VacBotSim main.cpp)

target_link_libraries(VacBotSim Box2D VacEnv MultiNEAT "${SIMPLE2D}" )
