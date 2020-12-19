#--------------------
# Global Set-Up
#--------------------
cmake_minimum_required(VERSION 3.14)

project("algo")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -g -O0 -fprofile-arcs -ftest-coverage --coverage")

include_directories("src")

#--------------------
# Components
#--------------------

# Configuration
set(CONFIG_SRC "src/configuration/ConfigurationFile.cpp")
add_library("configuration" STATIC ${CONFIG_SRC})

# Utilities
set(UTILITIES_SRC "src/utils/string.cpp")
add_library("utilities" STATIC ${UTILITIES_SRC})

# Graph
set(GRAPH_SRC "src/graph/GraphImpl.cpp")
add_library("basic_graph" STATIC ${GRAPH_SRC})

# Species
set(SPECIES_SRC 
    "src/species/AttributeFactory.cpp"
    "src/species/Species.cpp")
add_library("species" STATIC ${SPECIES_SRC})

# Types
set(TYPES_SRC
    "src/types/Propability.cpp"
    "src/types/NodeId.cpp")
add_library("types" STATIC ${TYPES_SRC})

# Main
set(MAIN_SRC "src/main.cpp"
    #"src/dynamics.cpp"
    "src/ContactNetwork.cpp"
    "src/SSA.cpp"
    #"src/Specie.cpp"
#    "src/SSA.cpp"
    #"src/Utility.cpp"
    #"src/configs/SimulationConfig.cpp"
    #"src/configs/Segments.cpp"
    #"src/graph/GraphImpl.cpp")
)

add_executable("algo" ${MAIN_SRC})
target_link_libraries("algo" 
    "configuration"
    "utilities" 
    "basic_graph"
    "species"
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
# found a lot at https://github.com/nlohmann/json
#--------------------
include(CTest)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()

