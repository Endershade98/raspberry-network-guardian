default:
    just test

configure:
    cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug

build:
    cmake --build build -j$(nproc)

test:
    cmake --build build -j$(nproc)
    cd build && ctest --output-on-failure

test-root:
    cmake --build build -j$(nproc)
    cd build && sudo ctest --output-on-failure

run:
    cmake --build build -j$(nproc)
    ./build/network_guardian

clean:
    rm -rf build

rebuild:
    just clean
    just configure
    just build

coverage:
    cmake -S . -B build -G Ninja -DENABLE_COVERAGE=ON
    cmake --build build
    cd build && ctest
    lcov --capture --directory . --output-file coverage.info
    genhtml coverage.info --output-directory coverage-html

docker-test:
    docker compose run --rm tests

docker-run:
    docker compose run --rm guardian