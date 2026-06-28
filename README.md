# Raspberry Network Guardian

> A modern C++17 Linux network monitoring daemon based on raw sockets, capable of capturing, parsing and tracking TCP connections in real time.

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)
![Linux](https://img.shields.io/badge/platform-Linux-green)
![CMake](https://img.shields.io/badge/build-CMake-success)
![Tests](https://img.shields.io/badge/tests-GoogleTest-success)
![Docker](https://img.shields.io/badge/docker-supported-blue)
![License](https://img.shields.io/badge/license-MIT-orange)

---

# Overview

**Raspberry Network Guardian** is a low-level network monitoring engine written in modern **C++17**.

The project implements a complete packet processing pipeline starting from Linux raw sockets and ending with a stateful TCP connection tracker.

Unlike traditional packet capture wrappers, the project directly interacts with the Linux networking stack through:

- `AF_PACKET` raw sockets
- custom packet parsing
- internal domain models
- TCP state tracking

The goal is to provide a production-oriented systems programming project focused on:

- Linux networking
- modern C++
- clean architecture
- testability
- deployment automation

---

# Features

Implemented:

- Linux raw socket capture
- Ethernet frame processing
- IPv4 parsing
- TCP header parsing
- Packet → domain mapping
- Stateful TCP connection tracking
- TCP state machine
- Graceful shutdown handling
- Thread-safe capture loop
- Unit testing
- Integration testing
- CTest pipeline
- Docker development environment
- Systemd production service
- CMake modular build system

---

# Architecture

```

```
             Linux Kernel

                  │

          AF_PACKET Socket

                  │

                  ▼

    +---------------------------+
    |     RawSocketSniffer      |
    +---------------------------+

                  │

                  ▼

    +---------------------------+
    |      PacketParser         |
    +---------------------------+

                  │

                  ▼

    +---------------------------+
    |      PacketMapper         |
    +---------------------------+

                  │

                  ▼

    +---------------------------+
    |   ConnectionTracker       |
    +---------------------------+

                  │

                  ▼

    +---------------------------+
    |     TCP StateMachine      |
    +---------------------------+
```

```

---

# Repository Structure

```

.
├── src
│
├── include
│
├── tests
│
├── cmake
│   ├── CompilerWarnings.cmake
│   ├── Sanitizers.cmake
│   ├── Coverage.cmake
│   └── Dependencies.cmake
│
├── docker
│   ├── Dockerfile.dev
│   └── docker-compose.yml
│
├── systemd
│   └── network-guardian.service
│
├── scripts
│
├── CMakeLists.txt
├── Justfile
└── README.md

```

---

# Packet Processing Pipeline

```

Raw Ethernet Frame

```
    │

    ▼
```

RawSocketSniffer

```
    │

    ▼
```

PacketParser

```
    │

    ▼
```

PacketMapper

```
    │

    ▼
```

ConnectionTracker

```
    │

    ▼
```

TCP State Machine

```

Every component has a single responsibility and can be tested independently.

---

# TCP State Machine

The engine tracks TCP connection lifecycle.

```

CLOSED

│

SYN

▼

SYN_RECEIVED

│

ACK

▼

ESTABLISHED

│

FIN

▼

FIN_WAIT

│

ACK

▼

CLOSED

````

RST packets immediately terminate a connection.

---

# Requirements

- Linux kernel
- GCC ≥ 11 or Clang
- CMake ≥ 3.20
- Ninja
- Docker
- Just


Install Just:

```bash
cargo install just
````

---

# Build

Clone:

```bash
git clone https://github.com/<username>/raspberry-network-guardian.git

cd raspberry-network-guardian
```

Configure:

```bash
just configure
```

Build:

```bash
just build
```

---

# Run

Raw socket capture requires elevated privileges.

Run locally:

```bash
sudo ./build/network_guardian eth0
```

or using Just:

```bash
sudo just run-root
```

Example:

```bash
sudo ./build/network_guardian wlan0
```

---

# Testing

Build and run tests:

```bash
just test
```

Direct unit tests:

```bash
just unit
```

CTest is used as the test runner.

---

# Sanitizers

Enable AddressSanitizer and UndefinedBehaviorSanitizer:

```bash
just configure-sanitize

just build

just test
```

Detects:

* memory errors
* invalid access
* undefined behaviour
* lifetime issues

---

# Coverage

Generate coverage data:

```bash
just coverage
```

Produces:

```
coverage.html
```

---

# Docker

Build development image:

```bash
just docker-build
```

Run tests inside container:

```bash
just docker-test
```

Run application:

```bash
just docker-run
```

Docker environment includes:

* CMake
* Ninja
* GoogleTest
* GCC toolchain
* debugging tools

---

# Production Deployment

The project includes a systemd service.

Install:

```bash
just build

just install
```

Enable:

```bash
just service-enable
```

Start:

```bash
just service-start
```

Check status:

```bash
just service-status
```

View logs:

```bash
just service-log
```

The daemon runs from:

```
/opt/network-guardian/network_guardian
```

---

# CMake Features

The build system supports:

* modular CMake files
* compiler warnings
* sanitizers
* coverage
* FetchContent dependencies
* Ninja builds
* install targets

Example:

```bash
cmake \
-S . \
-B build \
-G Ninja \
-DENABLE_SANITIZERS=ON
```

---

# Development Workflow

Typical development cycle:

```bash
just configure

just build

just test

just run
```

Full validation:

```bash
just ci
```

---

# Technologies

* C++17
* Linux Raw Sockets
* POSIX
* Threads
* GoogleTest
* CMake
* Ninja
* Docker
* Systemd
* Redis (planned)
* SQLite (planned)

---

# Roadmap

Planned:

* Redis event streaming
* SQLite persistence
* packet injection
* configuration file
* structured logging
* Prometheus metrics
* REST API
* distributed monitoring mode

---

# Engineering Goals

This project demonstrates:

* systems programming
* Linux networking
* modern C++ architecture
* clean separation of concerns
* automated testing
* production deployment practices

---

# License

MIT License

---

# Author

Developed as a portfolio project focused on:

* modern C++
* backend engineering
* Linux systems programming
* network infrastructure
