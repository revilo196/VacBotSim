cmake_minimum_required(VERSION 3.12)
project(VacBotSim)

set(CMAKE_CXX_STANDARD 14)

#find_library(SIMPLE2D simple2d)

find_package(PythonLibs REQUIRED)
include(FindPythonLibs)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(C:\\Users\\Oliver\\AppData\\Local\\simple2d\\include)

set(BOOST_ROOT C:/local/boost)
set(BOOST_LIBRARYDIR C:/local/boost/stage/lib/)
set (BOOST_INCLUDEDIR C:/local/boost)

include_directories(C:/local/boost)
link_directories(C:/local/boost/stage/lib/)
#link_directories(C:/Users/Oliver/AppData/Local/simple2d/lib)
#link_directories(C:/Users/Oliver/AppData/Local/simple2d)

link_directories(C:/local/simple2d)
add_subdirectory(./Box2D)
add_subdirectory(./VacEnv)
#add_subdirectory(./MultiNEAT)

include_directories(${PYTHON_INCLUDE_DIRS})



set(SOURCE_FILES  MultiNEAT/Assert.h
        MultiNEAT/Genes.h
        MultiNEAT/Genome.cpp
        MultiNEAT/Genome.h
        MultiNEAT/Innovation.cpp
        MultiNEAT/Innovation.h
        #MultiNEAT/Main.cpp
        MultiNEAT/NeuralNetwork.cpp
        MultiNEAT/NeuralNetwork.h
        MultiNEAT/Parameters.cpp
        MultiNEAT/Parameters.h
        MultiNEAT/PhenotypeBehavior.cpp
        MultiNEAT/PhenotypeBehavior.h
        MultiNEAT/Population.cpp
        MultiNEAT/Population.h
        #PythonBindings.cpp
        MultiNEAT/Random.cpp
        MultiNEAT/Random.h
        MultiNEAT/Species.cpp
        MultiNEAT/Species.h
        MultiNEAT/Substrate.cpp
        MultiNEAT/Substrate.h
        MultiNEAT/Traits.cpp
        MultiNEAT/Traits.h
        MultiNEAT/Utils.cpp
        MultiNEAT/Utils.h )

add_executable(VacBotSim main.cpp ${SOURCE_FILES})
target_link_libraries(VacBotSim   Box2D VacEnv simple2d  libboost_date_time-vc141-mt-x64-1_74
        libboost_system-vc141-mt-x64-1_74
        libboost_filesystem-vc141-mt-x64-1_74
        libboost_regex-vc141-mt-x64-1_74
        libboost_thread-vc141-mt-x64-1_74
        libboost_serialization-vc141-mt-x64-1_74
        )
