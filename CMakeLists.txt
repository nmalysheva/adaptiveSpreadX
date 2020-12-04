#--------------------
# Global Set-Up
#--------------------
cmake_minimum_required(VERSION 3.14)

project("algo")


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic")

#--------------------
# 3rd Party LEMON
#--------------------
FIND_PATH(LEMON_SOURCE_ROOT_DIR CMakeLists.txt
        PATHS ${CMAKE_SOURCE_DIR}/lib/lemon-1.3.1
        NO_DEFAULT_PATH
        DOC "Location of LEMON source as a CMAKE subproject")

IF(EXISTS ${LEMON_SOURCE_ROOT_DIR})
    ADD_SUBDIRECTORY(${LEMON_SOURCE_ROOT_DIR} deps/lemon)
    SET(LEMON_INCLUDE_DIRS
            ${LEMON_SOURCE_ROOT_DIR}
            ${CMAKE_BINARY_DIR}/deps/lemon
            )
    SET(LEMON_LIBRARIES lemon)
    UNSET(LEMON_ROOT_DIR CACHE)
    UNSET(LEMON_DIR CACHE)
    UNSET(LEMON_INCLUDE_DIR CACHE)
    UNSET(LEMON_LIBRARY CACHE)
ELSE()
    FIND_PACKAGE(LEMON QUIET NO_MODULE)
    FIND_PACKAGE(LEMON REQUIRED)
ENDIF()

INCLUDE_DIRECTORIES(
        #${PROJECT_SOURCE_DIR}
        #${PROJECT_BINARY_DIR}
        ${LEMON_INCLUDE_DIRS}
)

set(SOURCES 
    "src/main.cpp"
    "src/ConfigurationFile.cpp"
    "src/dynamics.cpp"
    "src/ContactNetwork.cpp"
    "src/Specie.cpp"
    "src/SSA.cpp"
    "src/Utility.cpp")

add_executable("algo" ${SOURCES})
target_compile_options("algo" PUBLIC -Wall -Werror -Wextra -pedantic)

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

