cmake_minimum_required(VERSION 3.9)
project(MultiNEAT)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++14")
set(BOOST_DISABLE_ASSERTS)
include_directories(.)
add_compile_definitions(BOOST_DISABLE_ASSERTS)
#find_package(PythonLibs REQUIRED)
#include(FindPythonLibs)

set(BOOST_ROOT "C:\\local\\boost_1_69_0gcc")
include_directories( "C:\\local\\boost_1_69_0gcc")
SET (BOOST_ROOT "C:\\local\\boost_1_69_0gcc")
SET (BOOST_INCLUDEDIR "C:\\local\\boost_1_69_0gcc\\boost")
SET (BOOST_LIBRARYDIR "C:\\local\\boost_1_69_0gcc\\libs")

set(Boost_USE_STATIC_LIBS       ON) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    ON)
find_package(Boost 1.69.0 REQUIRED COMPONENTS
        date_time
        system
        filesystem
        regex
        thread
        # python3
        serialization)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

#(${PYTHON_INCLUDE_DIRS})

#add_definitions(-DUSE_BOOST_PYTHON)
add_definitions(-DUSE_BOOST_RANDOM)


set(SOURCE_FILES Assert.h
        Genes.h
        Genome.cpp
        Genome.h
        Innovation.cpp
        Innovation.h
        #Main.cpp
        NeuralNetwork.cpp
        NeuralNetwork.h
        Parameters.cpp
        Parameters.h
        PhenotypeBehavior.cpp
        PhenotypeBehavior.h
        Population.cpp
        Population.h
        #  PythonBindings.cpp
        Random.cpp
        Random.h
        Species.cpp
        Species.h
        Substrate.cpp
        Substrate.h
        Traits.cpp
        Traits.h
        Utils.cpp
        Utils.h )

add_library(MultiNEAT ${SOURCE_FILES})
target_link_libraries(MultiNEAT ${Boost_LIBRARIES}  )
#${PYTHON_LIBRARIES}