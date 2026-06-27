#!/usr/bin/env bash

set -e

rm -rf build

cmake \
    -S . \
    -B build \
    -G Ninja \
    -DENABLE_COVERAGE=ON

cmake --build build

./build/all_tests

cd build

lcov \
    --capture \
    --directory . \
    --ignore-errors mismatch \
    --output-file coverage.info

lcov \
    --remove coverage.info \
    "/usr/*" \
    "*/tests/*" \
    "*/build/_deps/*" \
    --output-file coverage.info

genhtml \
    coverage.info \
    --output-directory coverage-html


echo
echo "Coverage generated:"
echo "build/coverage-html/index.html"