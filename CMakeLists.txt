#--------------------
# Global Set-Up
#--------------------
cmake_minimum_required(VERSION 3.14)

project("algo")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


if (NOT CMAKE_BUILD_TYPE)
    message("Build type not set: will be changed to Debug")
    set(CMAKE_BUILD_TYPE "Debug")
endif()


set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wunreachable-code -Wall -Wextra -pedantic -g -O0 -fprofile-arcs -ftest-coverage --coverage")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra -pedantic -Werror")


#--------------------
# Documentation
#--------------------
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_FULL_PATH_NAMES "YES")
    set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/doc")
    set(DOXYGEN_STRIP_FROM_PATH "${CMAKE_SOURCE_DIR}/src")
    doxygen_add_docs(doc "${CMAKE_SOURCE_DIR}/src/")
else()
    message(WARNING "Doxygen not found: Cannot create documentation (target doc)")
endif()

#--------------------
# Components
#--------------------

include_directories("src")

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
    message(STATUS "Enabling tests")
    enable_testing()
    add_subdirectory(test)
else()
    MESSAGE(STATUS "Disabling tests")
endif()


#--------------------
# Static Code Analysis
#--------------------
find_program(CLANG_TIDY clang-tidy)
if(${CLANG_TIDY} STREQUAL "CLANG_TIDY-NOTFOUND")
    message(WARNING "clang-tidy not found. Cannot use target staticanalysis")
else()
    message(STATUS "clang-tidy found. Enabling target staticanalysis")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    add_custom_target(staticanalysis
        COMMAND clang-tidy -p=${CMAKE_BINARY_DIR} -warnings-as-errors=* -checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*,modernize-*,readability-*,performance-*,portability-*,-cppcoreguidelines-pro-bounds-array-to-pointer-decay,-readability-uppercase-literal-suffix `find ${CMAKE_SOURCE_DIR}/src -iname \"*.[ch]pp\"` -extra-arg=-std=c++17)
endif()
