#!/usr/bin/env bash

set -e

find include src tests \
    \( \
        -name "*.hpp" \
        -o -name "*.cpp" \
    \) \
    -print0 \
| xargs -0 clang-format -i