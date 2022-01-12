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
option(BUILD_DOC
       "Enable code documentation generation with doxygen (target: doc)"
       ON)

if(BUILD_DOC)
    message(STATUS "Enabling doxygen")
    find_package(Doxygen REQUIRED)
    set(DOXYGEN_FULL_PATH_NAMES "YES")
    set(DOXYGEN_EXTRACT_ALL "YES")
    set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/doc")
    set(DOXYGEN_STRIP_FROM_PATH "${CMAKE_SOURCE_DIR}/src")
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md")
    doxygen_add_docs(doc "${CMAKE_SOURCE_DIR}/README.md" "${CMAKE_SOURCE_DIR}/src/")
else()
    message(STATUS "Disabling doxygen")
endif()



#--------------------
# Components
#--------------------

include_directories("src")


# Types
set(TYPES_SRC
    "src/types/Distribution.cpp"
    "src/types/DistributionImpl.cpp"
    "src/types/NodeId.cpp"
    "src/types/State.cpp")
add_library("types" STATIC ${TYPES_SRC})


# Utils
set(UTILS_SRC
    "src/utils/Json.cpp"
    "src/utils/Parse.cpp"
    "src/utils/Random.cpp")
add_library("utils" STATIC ${UTILS_SRC})
target_link_libraries("utils" "types")


# Configuration
set(CONFIG_SRC 
    "src/configuration/Configuration.cpp"
    "src/configuration/Exception.cpp"
    "src/configuration/Helper.cpp"
    "src/configuration/Stream.cpp")
add_library("configuration" STATIC ${CONFIG_SRC})
target_link_libraries("configuration" "utils" "types")

# Graph
set(GRAPH_SRC "src/graph/GraphImpl.cpp")
add_library("basic_graph" STATIC ${GRAPH_SRC})
target_link_libraries("basic_graph" "types" "utils")

# Network    
set(NETWORK_SRC 
    "src/network/ContactNetwork.cpp"
    "src/network/IndividualFactory.cpp"
    "src/network/Interaction.cpp"
    "src/network/InteractionManager.cpp"
    "src/network/Settings.cpp")
add_library("network" STATIC ${NETWORK_SRC})
target_link_libraries("network" "configuration" "types" "basic_graph")

# Algorithm
set(ALGORITHM_SRC
    "src/algorithm/ActionsFactory.cpp"
    "src/algorithm/Algorithm.cpp"
    #    "src/algorithm/AlgorithmImpl.cpp"
    "src/algorithm/Settings.cpp"
    "src/algorithm/SSA.cpp"
    "src/algorithm/SSATANX.cpp"
    "src/algorithm/TauLeap.cpp")
add_library("algorithm" STATIC ${ALGORITHM_SRC})
target_link_libraries("algorithm" "types" "network" "utils")

# Main
set(MAIN_SRC "src/main.cpp") 
add_executable("algo" ${MAIN_SRC})
target_link_libraries("algo"
    "algorithm"
    "configuration"
    "network"
    "utils")

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

option(BUILD_STATICANALYSIS
       "Enable static code analysis with clang-tidy (target: staticanalysis)"
       OFF)

if (BUILD_STATICANALYSIS)
    message(STATUS "Enabling static code analysis")

    find_program(CLANG_TIDY clang-tidy)
    if(${CLANG_TIDY} STREQUAL "CLANG_TIDY-NOTFOUND")
        message(FATAL_ERROR "clang-tidy not found. Cannot use target staticanalysis")
    endif()

    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    add_custom_target(staticanalysis
        COMMAND clang-tidy -p=${CMAKE_BINARY_DIR} -warnings-as-errors=* -checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*,modernize-*,readability-*,performance-*,portability-*,-cppcoreguidelines-pro-bounds-array-to-pointer-decay,-readability-uppercase-literal-suffix,-cppcoreguidelines-special-member-functions,-cppcoreguidelines-non-private-member-variables-in-classes,-cppcoreguidelines-pro-bounds-constant-array-index `find ${CMAKE_SOURCE_DIR}/src -iname \"*.[ch]pp\"` -extra-arg=-std=c++17)
else()
    message(STATUS "Disabling static code analysis")
endif()

