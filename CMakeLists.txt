cmake_minimum_required(VERSION 3.20)
project(raspberry_network_guardian VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_TESTS "Build tests" ON)
option(ENABLE_COVERAGE "Enable coverage flags" OFF)

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
    -Wshadow
    -Wconversion
)

if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(--coverage -O0 -g)
    add_link_options(--coverage)
endif()

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(googletest)

enable_testing()

include_directories(src)

set(APP_SOURCES
    src/application/ConnectionTracker.cpp
    src/application/PacketParser.cpp
    src/infrastructure/PacketInjector.cpp
    src/infrastructure/RawSocketSniffer.cpp
    src/infrastructure/RedisClient.cpp
    src/infrastructure/SQLiteRepository.cpp
    src/interfaces/CLI.cpp
    src/interfaces/Logger.cpp
)

add_executable(network_guardian
    ${APP_SOURCES}
    src/main.cpp
)

target_link_libraries(network_guardian pthread)

if(ENABLE_TESTS)

set(TEST_SOURCES
    tests/unit/test_connection.cpp
    tests/unit/test_connection_tracker.cpp
    tests/unit/test_packet.cpp
    tests/unit/test_parser.cpp
    tests/unit/test_raw_socket_sniffer.cpp
    tests/unit/test_state_machine.cpp
    tests/unit/test_tracker.cpp
    tests/integration/test_sniffer_loopback.cpp
    tests/integration/test_sniffer_start_stop.cpp
    tests/integration/test_sniffer_parser.cpp
    tests/integration/test_tracker_redis.cpp
    tests/e2e/test_full_pipeline.cpp
)

add_executable(test_runner
    ${APP_SOURCES}
    src/application/ConnectionTracker.cpp
    src/application/PacketParser.cpp
    ${TEST_SOURCES}
)

target_link_libraries(test_runner
    GTest::gtest_main
    pthread
)

include(GoogleTest)
gtest_discover_tests(test_runner)

endif()