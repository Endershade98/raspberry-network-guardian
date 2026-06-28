####################################################
# DEFAULT
####################################################

default:
    just test


####################################################
# CONFIGURATION
####################################################

configure:
    cmake -S . -B build -G Ninja


configure-release:
    cmake \
        -S . \
        -B build \
        -G Ninja \
        -DCMAKE_BUILD_TYPE=Release


configure-debug:
    cmake \
        -S . \
        -B build \
        -G Ninja \
        -DCMAKE_BUILD_TYPE=Debug


configure-sanitize:
    cmake \
        -S . \
        -B build \
        -G Ninja \
        -DENABLE_SANITIZERS=ON


configure-coverage:
    cmake \
        -S . \
        -B build \
        -G Ninja \
        -DENABLE_COVERAGE=ON



####################################################
# BUILD
####################################################

build:
    cmake --build build


clean:
    rm -rf build


rebuild:
    just clean
    just configure
    just build



####################################################
# RUN
####################################################

run:
    ./build/network_guardian lo


run-root:
    sudo ./build/network_guardian lo



####################################################
# TESTS
####################################################

test:
    just build
    cd build && ctest --output-on-failure


tests:
    just test


unit:
    ./build/all_tests


unit-root:
    sudo ./build/all_tests



####################################################
# STATIC ANALYSIS
####################################################

warnings:
    cmake \
        -S . \
        -B build \
        -DENABLE_SANITIZERS=ON

    cmake --build build



####################################################
# COVERAGE
####################################################

coverage:
    just configure-coverage
    just build
    ./scripts/coverage.sh

####################################################
# FORMAT
####################################################

format:
    ./scripts/format.sh


format-check:
    clang-format \
        --dry-run \
        --Werror \
        $(find src include tests -name '*.cpp' -o -name '*.hpp')



####################################################
# DOCKER
####################################################

docker-build:
    docker compose \
        -f docker/docker-compose.yml \
        build


docker-test:
    docker compose \
        -f docker/docker-compose.yml \
        run --rm build


docker-run:
    docker compose \
        -f docker/docker-compose.yml \
        run --rm guardian



####################################################
# SYSTEMD DEPLOY
####################################################

install:
    sudo cmake --install build


service-enable:
    sudo systemctl daemon-reload
    sudo systemctl enable network-guardian


service-start:
    sudo systemctl start network-guardian


service-stop:
    sudo systemctl stop network-guardian


service-status:
    sudo systemctl status network-guardian


service-log:
    sudo journalctl \
        -u network-guardian \
        -f



####################################################
# FULL PIPELINE
####################################################

ci:
    just clean
    just configure
    just build
    just test


release-check:
    just clean
    just configure-release
    just build
    just test