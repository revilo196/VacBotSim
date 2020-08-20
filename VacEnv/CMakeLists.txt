cmake_minimum_required(VERSION 3.12)
project(VacEnv VERSION 1.0.1 DESCRIPTION "Roboter Simulation")
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)


include_directories(.)
include_directories(C:\\Users\\Oliver\\AppData\\Local\\simple2d\\.)
include_directories(C:\\Users\\Oliver\\AppData\\Local\\simple2d\\SDL2)


link_directories(C:/local/simple2d/lib)

add_library(VacEnv VacBot.cpp RoomWorld.cpp VacEnv.cpp VacEnvWindow.cpp)
add_library(VacEnvS SHARED  VacBot.cpp RoomWorld.cpp VacEnv.cpp VacEnvWindow.cpp )
add_library(VacEnvNoUI SHARED  VacBot.cpp RoomWorld.cpp VacEnv.cpp )

set_target_properties(VacEnvS PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VacEnvS PROPERTIES PUBLIC_HEADER VacEnv.h)

# SDL2_image SDL2_mixer SDL2_ttf SDL2main
target_include_directories(VacEnvS PRIVATE .)
TARGET_LINK_LIBRARIES(VacEnv  Box2D simple2d SDL2 opengl32 glew32 )
TARGET_LINK_LIBRARIES(VacEnvS  Box2D simple2d SDL2 opengl32 glew32     )
TARGET_LINK_LIBRARIES(VacEnvNoUI  Box2D)



FILE(COPY __init__.py DESTINATION .)
FILE(COPY VacEnvWrapper.py DESTINATION .)
FILE(COPY VacTest.py DESTINATION .)
FILE(COPY VacWindowWrapper.py DESTINATION .)
FILE(COPY VacEnvGym.py DESTINATION .)
