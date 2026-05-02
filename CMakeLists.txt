cmake_minimum_required(VERSION 3.16)
project(NetworkGuardian)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Warnings
add_compile_options(-Wall -Wextra -Wpedantic)

# ------------------------
# Includes
# ------------------------
include_directories(src)

# ------------------------
# Main App
# ------------------------
file(GLOB_RECURSE APP_SOURCES
    src/*.cpp
)

# Exclude main from tests later if needed
add_executable(network_guardian ${APP_SOURCES})

# ------------------------
# GoogleTest
# ------------------------
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# ------------------------
# Tests
# ------------------------
enable_testing()

file(GLOB_RECURSE TEST_SOURCES
    tests/**/*.cpp
    tests/*.cpp
)

# Exclude src/main.cpp from test binary
list(FILTER APP_SOURCES EXCLUDE REGEX "src/main.cpp")

add_executable(test_runner
    ${TEST_SOURCES}
    ${APP_SOURCES}
)

target_link_libraries(test_runner
    GTest::gtest
    GTest::gtest_main
    pthread
)

include(GoogleTest)

gtest_discover_tests(test_runner)