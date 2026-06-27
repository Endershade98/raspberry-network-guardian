default:
    just test

configure:
    cmake -S . -B build -G Ninja

build:
    cmake --build build -j$(nproc)

clean:
    rm -rf build

rebuild:
    just clean
    just configure
    just build

run:
    just build
    ./build/network_guardian

####################################################
# TESTS
####################################################

test:
    just build
    cd build && ctest --output-on-failure

unit:
    just build
    ./build/unit_tests

integration:
    just build
    ./build/integration_tests

integration-root:
    just build
    sudo ./build/integration_tests

e2e:
    just build
    sudo ./build/e2e_tests

test-root:
    just build
    cd build && sudo ctest --output-on-failure

####################################################
# COVERAGE
####################################################

coverage:
    rm -rf build
    cmake -S . -B build -G Ninja -DENABLE_COVERAGE=ON
    cmake --build build
    ./build/unit_tests

    cd build && \
    lcov --capture --directory . --output-file coverage.info && \
    genhtml coverage.info --output-directory coverage-html

####################################################
# DOCKER
####################################################

docker-test:
    docker compose run --rm tests

docker-run:
    docker compose run --rm guardian