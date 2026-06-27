#!/usr/bin/env bash
set -e

cmake -S . -B build -G Ninja
cmake --build build -j$(nproc)
cd build
ctest --output-on-failure