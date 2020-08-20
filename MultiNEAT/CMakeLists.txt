cmake_minimum_required(VERSION 3.11)
project(MultiNEAT)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++14")
set(BOOST_DISABLE_ASSERTS)
include_directories(.)
add_compile_definitions(BOOST_DISABLE_ASSERTS)
find_package(PythonLibs REQUIRED)
include(FindPythonLibs)

set(Boost_USE_STATIC_LIBS       ON) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

set(BOOST_ROOT C:/local/boost_1_69_0)
set(BOOST_LIBRARYDIR C:/local/boost_1_69_0/stage/lib)
set (BOOST_INCLUDEDIR C:/local/boost_1_69_0)

#find_package(Boost REQUIRED COMPONENTS
 #       date_time
 #       system
#        filesystem
#        regex
#        thread
#        # python3
#        serialization)
#if(Boost_FOUND)
#    include_directories(${Boost_INCLUDE_DIRS})
#endif()

include_directories(${BOOST_INCLUDEDIR})
link_directories(${BOOST_LIBRARYDIR})

include_directories(${PYTHON_INCLUDE_DIRS})

#add_definitions(-DUSE_BOOST_PYTHON)
add_definitions(-DUSE_BOOST_RANDOM)


set(SOURCE_FILES Assert.h
        Genes.h
        Genome.cpp
        Genome.h
        Innovation.cpp
        Innovation.h
        Main.cpp
        NeuralNetwork.cpp
        NeuralNetwork.h
        Parameters.cpp
        Parameters.h
        PhenotypeBehavior.cpp
        PhenotypeBehavior.h
        Population.cpp
        Population.h
        PythonBindings.cpp
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
target_link_libraries(MultiNEAT -static ${Boost_LIBRARIES} boost_date_time
        boost_system
        boost_filesystem
        boost_python
        boost_regex
        boost_thread
        boost_serialization)
#${PYTHON_LIBRARIES}