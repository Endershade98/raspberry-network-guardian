cmake_minimum_required(VERSION 3.16)
project(NetworkGuardian)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable warnings
add_compile_options(-Wall -Wextra -Wpedantic)

# Include directories
include_directories(src)

# Source files
file(GLOB_RECURSE SOURCES src/*.cpp)

# Main executable
add_executable(network_guardian ${SOURCES})

# ------------------------
# Testing
# ------------------------
enable_testing()

# Add tests
file(GLOB_RECURSE TEST_SOURCES tests/*.cpp)

add_executable(test_runner ${TEST_SOURCES})

add_test(NAME AllTests COMMAND test_runner)