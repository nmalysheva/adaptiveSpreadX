#-------------------- # Global Set-Up
#--------------------
cmake_minimum_required(VERSION 3.14)

project("algo")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunreachable-code -Wall -Wextra -pedantic -g -O0 -fprofile-arcs -ftest-coverage --coverage")

include_directories("src")

#--------------------
# Components
#--------------------

# Types
set(TYPES_SRC
    "src/types/Distribution.cpp"
    "src/types/State.cpp"
    "src/types/NodeId.cpp")
add_library("types" STATIC ${TYPES_SRC})

set(UTILS_SRC
    "src/utils/Parse.cpp")
add_library("utils" STATIC ${UTILS_SRC})
target_link_libraries("utils"
    "types")

# Configuration
set(CONFIG_SRC 
    "src/configuration/Configuration.cpp"
    "src/configuration/Exception.cpp"
    "src/configuration/Helper.cpp"
    "src/configuration/Stream.cpp")
add_library("configuration" STATIC ${CONFIG_SRC})
target_link_libraries("configuration" "utils")

# Settings
set(SETTINGS_SRC
    "src/settings/Settings.cpp"
    "src/settings/algorithm/Interaction.cpp"
    "src/settings/algorithm/Transition.cpp"
    "src/settings/algorithm/Transformation.cpp")
add_library("settings" STATIC ${SETTINGS_SRC})
target_link_libraries("settings" "types")

# Graph
set(GRAPH_SRC "src/graph/GraphImpl.cpp")
add_library("basic_graph" STATIC ${GRAPH_SRC})
target_link_libraries("basic_graph" types)
    
set(NETWORK_SRC 
    "src/network/ContactNetwork.cpp")
add_library("network" STATIC ${NETWORK_SRC})
target_link_libraries("network" 
    "types")

set(ALGORITHM_SRC
    "src/algorithm/SSA.cpp")
add_library("algorithm" STATIC ${ALGORITHM_SRC})
target_link_libraries("algorithm"
    "types")

# Main
set(MAIN_SRC "src/main.cpp"
    #"src/dynamics.cpp"
#"src/ContactNetwork.cpp"
#"src/SSA.cpp"
    #"src/Specie.cpp"
#    "src/SSA.cpp"
    #"src/Utility.cpp"
    #"src/configs/SimulationConfig.cpp"
    #"src/configs/Segments.cpp"
    #"src/graph/GraphImpl.cpp")
)

add_executable("algo" ${MAIN_SRC})
target_link_libraries("algo" 
    "settings"
    "configuration"
    "algorithm"
    "basic_graph"
    "network"
    "utils"
    "types")

#--------------------
# Documentation
#--------------------
#find_package(Doxygen)
#if(DOXYGEN_FOUND)
#    set(DOXYGEN_FULL_PATH_NAMES "YES")
#    set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/doc")
#    set(DOXYGEN_STRIP_FROM_PATH "${CMAKE_SOURCE_DIR}/src")
#    set(DOXYGEN_PLANTUML_JAR_PATH "/usr/share/plantuml/plantuml.jar")
#    set(DOXYGEN_ENABLE_PREPROCESSING "YES")
#    set(DOXYGEN_PREDEFINED "BOSSWESTFALEN_ONLY_FOR_DOXYGEN")
#    doxygen_add_docs(doc "${CMAKE_SOURCE_DIR}/src/${SOURCE_FILE_NAME}")
#    # Custom script to wipe the content of the doc folder (see cmake/wipe_doc.cmake for more details)
#    add_custom_target(wipe_docs COMMAND ${CMAKE_COMMAND} -P "${CMAKE_SOURCE_DIR}/cmake/wipe_doc.cmake")
#    # wipe doc/html before generating documentation
#    add_dependencies(doc wipe_docs)
#endif()

#--------------------
# Test
#--------------------
include(CTest)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()

