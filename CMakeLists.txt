cmake_minimum_required(VERSION 3.20)

project(raspberry_network_guardian LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_TESTS "Build tests" ON)
option(ENABLE_COVERAGE "Coverage" OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
endif()

add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
)

if(ENABLE_COVERAGE)
    add_compile_options(--coverage -O0 -g)
    add_link_options(--coverage)
endif()

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

include(GoogleTest)

include_directories(src)

#################################################
# SOURCES
#################################################

set(APP_SOURCES
    src/application/ConnectionTracker.cpp
    src/application/PacketParser.cpp
    src/application/PacketMapper.cpp
    src/infrastructure/PacketInjector.cpp
    src/infrastructure/RawSocketSniffer.cpp
    src/infrastructure/RedisClient.cpp
    src/infrastructure/SQLiteRepository.cpp
    src/interfaces/CLI.cpp
    src/interfaces/Logger.cpp
)

#################################################
# MAIN
#################################################

add_executable(network_guardian

    ${APP_SOURCES}
    src/main.cpp
)

target_link_libraries(network_guardian pthread)

#################################################
# TESTS
#################################################

if(ENABLE_TESTS)

#################################
# UNIT
#################################

file(GLOB UNIT_TESTS
    tests/unit/*.cpp
)

add_executable(unit_tests

    ${APP_SOURCES}
    ${UNIT_TESTS}
)

target_link_libraries(unit_tests

    GTest::gtest_main
    pthread
)

gtest_discover_tests(unit_tests)

#################################
# INTEGRATION
#################################

file(GLOB INTEGRATION_TESTS
    tests/integration/*.cpp
)

add_executable(integration_tests

    ${APP_SOURCES}
    ${INTEGRATION_TESTS}
)

target_link_libraries(integration_tests

    GTest::gtest_main
    pthread
)

gtest_discover_tests(integration_tests)

#################################
# E2E
#################################

file(GLOB E2E_TESTS
    tests/e2e/*.cpp
)

add_executable(e2e_tests

    ${APP_SOURCES}
    ${E2E_TESTS}
)

target_link_libraries(e2e_tests

    GTest::gtest_main
    pthread
)

gtest_discover_tests(e2e_tests)

endif()