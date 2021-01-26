#--------------------
# Global Set-Up
#--------------------
cmake_minimum_required(VERSION 3.14)

project("algo")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunreachable-code -Wall -Wextra -pedantic -g -O0 -fprofile-arcs -ftest-coverage --coverage")

include_directories("src")

#--------------------
# Documentation
#--------------------
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_FULL_PATH_NAMES "YES")
    set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/doc")
    set(DOXYGEN_STRIP_FROM_PATH "${CMAKE_SOURCE_DIR}/src")
    doxygen_add_docs(doc "${CMAKE_SOURCE_DIR}/src/")
endif()

#--------------------
# Components
#--------------------

# Types
set(TYPES_SRC
    "src/types/Distribution.cpp"
    "src/types/State.cpp")
add_library("types" STATIC ${TYPES_SRC})

# Configuration
set(CONFIG_SRC 
    "src/configuration/Configuration.cpp"
    "src/configuration/Exception.cpp"
    "src/configuration/Helper.cpp"
    "src/configuration/Stream.cpp")
add_library("configuration" STATIC ${CONFIG_SRC})

# Graph
set(GRAPH_SRC "src/graph/GraphImpl.cpp")
add_library("basic_graph" STATIC ${GRAPH_SRC})
target_link_libraries("basic_graph"
    "types")

# Network    
set(NETWORK_SRC 
    "src/network/ContactNetwork.cpp"
    "src/network/Individual.cpp"
    "src/network/IndividualFactory.cpp"
    "src/network/NodeId.cpp"
    "src/network/Settings.cpp")
add_library("network" STATIC ${NETWORK_SRC})
target_link_libraries("network"
    "types" "basic_graph")

# Algorithm
set(ALGORITHM_SRC
    "src/algorithm/Interaction.cpp"
    "src/algorithm/Transition.cpp"
    "src/algorithm/Transformation.cpp"
    "src/algorithm/Settings.cpp"
    "src/algorithm/SSA.cpp")
add_library("algorithm" STATIC ${ALGORITHM_SRC})
target_link_libraries("algorithm"
    "types" "network")

# Settings
set(SETTINGS_SRC
    "src/settings/Parse.cpp"
    "src/settings/Settings.cpp")
add_library("settings" STATIC ${SETTINGS_SRC})
target_link_libraries("settings" 
    "network" "algorithm" "types")

# Main
set(MAIN_SRC "src/main.cpp") 
add_executable("algo" ${MAIN_SRC})
target_link_libraries("algo"
    "algorithm"
    "configuration"
    "network"
    "settings")

#--------------------
# Test
#--------------------
include(CTest)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()

