#!/usr/bin/env bash

set -e

cmake --build build

cd build

ctest --output-on-failure