cmake_minimum_required(VERSION 3.20)

project(
    raspberry_network_guardian
    VERSION 0.1.0
    DESCRIPTION "Real-time network monitoring engine"
    LANGUAGES CXX
)

############################################################
# OPTIONS
############################################################

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_TESTS "Build tests" ON)
option(ENABLE_COVERAGE "Enable coverage" OFF)
option(
    ENABLE_SANITIZERS
    "Enable address and undefined sanitizers"
    OFF
)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

############################################################
# CCACHE
############################################################

find_program(CCACHE_PROGRAM ccache)

if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
endif()

############################################################
# CMAKE MODULES
############################################################

include(cmake/CompilerWarnings.cmake)
include(cmake/Sanitizers.cmake)
include(cmake/Coverage.cmake)
include(cmake/Dependencies.cmake)

############################################################
# THREADS
############################################################

find_package(Threads REQUIRED)

############################################################
# INCLUDE
############################################################

include_directories(include)

############################################################
# SOURCES
############################################################

set(CORE_SOURCES

    src/application/ConnectionTracker.cpp
    src/application/PacketMapper.cpp
    src/application/PacketParser.cpp

    src/domain/StateMachine.cpp

    src/infrastructure/PacketInjector.cpp
    src/infrastructure/RawSocketSniffer.cpp
    src/infrastructure/RedisClient.cpp
    src/infrastructure/SQLiteRepository.cpp

    src/interfaces/CLI.cpp
    src/interfaces/Logger.cpp
)

############################################################
# LIBRARY
############################################################

add_library(network_guardian_core STATIC
    ${CORE_SOURCES}
)

target_enable_warnings(
    network_guardian_core
)


if(ENABLE_SANITIZERS)

    enable_sanitizers(
        network_guardian_core
    )

endif()


if(ENABLE_COVERAGE)

    enable_coverage(
        network_guardian_core
    )

endif()

target_include_directories(network_guardian_core
    PUBLIC include
)

target_link_libraries(network_guardian_core
    PUBLIC Threads::Threads
)

############################################################
# EXECUTABLE
############################################################

add_executable(network_guardian
    src/main.cpp
)

enable_coverage(
    network_guardian
)

target_enable_warnings(
    network_guardian
)

target_link_libraries(network_guardian
    PRIVATE network_guardian_core
)

############################################################
# TESTS
############################################################

if(ENABLE_TESTS)


add_project_dependencies()


enable_testing()


include(GoogleTest)


file(GLOB_RECURSE TEST_SOURCES
    tests/*.cpp
)


add_executable(all_tests
    ${TEST_SOURCES}
)

enable_coverage(
    all_tests
)

target_link_libraries(all_tests

    PRIVATE

    network_guardian_core
    GTest::gtest_main

)


gtest_discover_tests(all_tests)


endif()

install(
    TARGETS network_guardian
    RUNTIME DESTINATION bin
)

install(
    DIRECTORY systemd/
    DESTINATION lib/systemd/system
)