cmake_minimum_required(VERSION 3.12)
project(Box2D)

set(CMAKE_CXX_STANDARD 11)

find_library(SIMPLE2D simple2d)

include_directories(.)
include_directories(C:\\Users\\Oliver\\AppData\\Local\\simple2d\\.)
include_directories(C:\\Users\\Oliver\\AppData\\Local\\simple2d\\SDL2)

LINK_DIRECTORIES(C:\\Users\\Oliver\\AppData\\Local\\simple2d\\lib)

add_library(VacEnv VacBot.cpp RoomWorld.cpp VacEnv.cpp VacEnvWindow.cpp)
TARGET_LINK_LIBRARIES(VacEnv "${SIMPLE2D}")