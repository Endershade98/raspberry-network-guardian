#!/usr/bin/env bash
set -e

cmake -S . -B build -G Ninja -DENABLE_COVERAGE=ON
cmake --build build -j$(nproc)
cd build
ctest

lcov --capture --directory . --output-file coverage.info
genhtml coverage.info --output-directory coverage-html