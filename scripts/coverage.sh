#!/usr/bin/env bash

set -e

BUILD_DIR="build"

echo "Running tests..."
ctest \
    --test-dir "$BUILD_DIR" \
    --output-on-failure


echo ""
echo "Coverage summary"
echo ""

gcovr \
    --root . \
    --object-directory "$BUILD_DIR" \
    --filter "src/application" \
    --filter "src/domain" \
    --filter "src/infrastructure" \
    --filter "src/interfaces" \
    --exclude "tests" \
    --exclude "build/_deps" \
    --exclude "src/main.cpp" \
    --txt \
    --txt-summary \
    --print-summary \
    --fail-under-line 65