#--------------------
# Global set-up
#--------------------
option(TEST_COVERAGE "Build test suite with code coverage information" ON)

if(TEST_COVERAGE)
    message(STATUS "Building with code coverage information")
    if(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        message(FATAL_ERROR "TEST_COVERAGE requires GCC.")
    endif()

    # Only use flags needed for code coverage
#set(CMAKE_CXX_FLAGS "--coverage -g -O0 -fprofile-arcs -ftest-coverage")
    #assume gcov is available
    set(INFO_FILE "code_coverage.info")
    add_custom_target(coverage_report
        COMMAND lcov -c -i -d ${CMAKE_BINARY_DIR} -o base.info
        COMMAND lcov --directory ${CMAKE_BINARY_DIR} -c --output-file run.info --gcov-tool gcov --rc lcov_branch_coverage=0
        COMMAND lcov -a base.info -a run.info -o ${INFO_FILE}
        COMMAND lcov --extract ${INFO_FILE} '${CMAKE_SOURCE_DIR}/src/*' --output-file ${INFO_FILE} --rc lcov_branch_coverage=0
        COMMAND lcov --remove ${INFO_FILE} '${CMAKE_SOURCE_DIR}/src/main.cpp' -o ${INFO_FILE}
        COMMAND genhtml --title "${PROJECT_NAME}" --no-function-coverage --legend --demangle-cpp --output-directory html  ${INFO_FILE}
        COMMENT "Generating HTML report test/html/index.html")
endif()
#--------------------
# Unit Tests
#--------------------
set(UNIT_DIR "${CMAKE_SOURCE_DIR}/test/src/")
set(TP_DIR "${CMAKE_SOURCE_DIR}/test/third_party")
add_library(catch_main OBJECT "${UNIT_DIR}/main.cpp")
include_directories("${TP_DIR}/catch")


set(TYPES_TEST_SRC 
    "${UNIT_DIR}/types/State.cpp"
    "${UNIT_DIR}/types/Distribution.cpp")
add_executable("types_test" $<TARGET_OBJECTS:catch_main> ${TYPES_TEST_SRC})
target_link_libraries("types_test" types)
add_test(NAME "unit_test_types" COMMAND "types_test")


set(NETWORK_TEST_SRC
    "${UNIT_DIR}/network/ContactNetwork.cpp"
    "${UNIT_DIR}/network/Individual.cpp"
    "${UNIT_DIR}/network/IndividualFactory.cpp"
    "${UNIT_DIR}/network/NodeId.cpp"
    "${UNIT_DIR}/network/Settings.cpp")
add_executable("network_test" $<TARGET_OBJECTS:catch_main> ${NETWORK_TEST_SRC})
target_link_libraries("network_test" network)
add_test(NAME "unit_test_network" COMMAND "network_test")


set(UTILS_TEST_SRC 
    "${UNIT_DIR}/utils/Random.cpp")
add_executable("utils_test" $<TARGET_OBJECTS:catch_main> ${UTILS_TEST_SRC})
add_test(NAME "unit_test_utils" COMMAND "utils_test")


set(SETTINGS_TEST_SRC
    "${UNIT_DIR}/settings/Parse.cpp"
    "${UNIT_DIR}/settings/Settings.cpp")
add_executable("settings_test" $<TARGET_OBJECTS:catch_main> ${SETTINGS_TEST_SRC})
target_link_libraries("settings_test" "settings")
add_test(NAME "unit_test_settings" COMMAND "settings_test")


set(CONFIG_TEST_SRC
    "${UNIT_DIR}/configuration/Configuration.cpp"
    "${UNIT_DIR}/configuration/Helper.cpp"
    "${UNIT_DIR}/configuration/Stream.cpp")
add_executable("config_test" $<TARGET_OBJECTS:catch_main> ${CONFIG_TEST_SRC})
target_link_libraries("config_test" configuration)
add_test(NAME "unit_test_config" COMMAND "config_test")

set(ALGORITHM_TEST_SRC
    "${UNIT_DIR}/algorithm/Actions.cpp"
    "${UNIT_DIR}/algorithm/Interaction.cpp"
    "${UNIT_DIR}/algorithm/SSA.cpp"
    "${UNIT_DIR}/algorithm/Settings.cpp"
    "${UNIT_DIR}/algorithm/Transformation.cpp"
    "${UNIT_DIR}/algorithm/Transition.cpp")
add_executable("algorithm_test" $<TARGET_OBJECTS:catch_main> ${ALGORITHM_TEST_SRC})
target_link_libraries("algorithm_test" 
    "algorithm" "configuration" "settings")
add_test(NAME "unit_test_algorithm" COMMAND "algorithm_test")

