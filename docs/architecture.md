# Software Architecture

## Overview

`raspberry-network-guardian` is a modular C++17 network monitoring system designed to capture, parse, analyze and track TCP traffic in real time.

The project follows a layered architecture inspired by:

* Clean Architecture
* Hexagonal Architecture (Ports & Adapters)
* Domain Driven Design principles

The main architectural goal is to isolate:

* network-specific implementation details
* packet processing logic
* domain models
* external integrations

This allows the system to evolve from a Raspberry Pi monitoring tool into a portable network observability platform.

---

# Architectural Goals

The architecture has been designed around the following principles:

## Separation of concerns

Each layer owns a specific responsibility.

Examples:

* raw sockets do not know about TCP states
* parsers do not know about persistence
* domain objects do not depend on Linux APIs

---

## Testability

Business logic must be executable without:

* real network interfaces
* root privileges
* external services

This enables:

* fast unit tests
* deterministic behavior
* easier debugging

---

## Portability

The project targets:

* Raspberry Pi ARM64
* Linux servers
* development environments

Infrastructure dependencies are isolated behind interfaces.

---

# High Level Architecture

```
                    +----------------------+
                    |      CLI / Main      |
                    +----------+-----------+
                               |
                               v

                    +----------------------+
                    |  Application Layer   |
                    |                      |
                    | PacketParser         |
                    | PacketMapper         |
                    | ConnectionTracker    |
                    | StateMachine         |
                    +----------+-----------+
                               |
                               v

                    +----------------------+
                    |   Domain Layer       |
                    |                      |
                    | Packet               |
                    | Connection           |
                    | ConnectionKey        |
                    | ConnectionState      |
                    +----------------------+


                               ^
                               |

                    +----------------------+
                    | Infrastructure Layer |
                    |                      |
                    | RawSocketSniffer     |
                    | PacketInjector       |
                    | RedisClient          |
                    | SQLiteRepository     |
                    +----------------------+

```

---

# Project Layers

## Domain Layer

Location:

```
src/domain/
```

The domain layer contains pure business objects.

It has no dependencies on:

* Linux system calls
* sockets
* databases
* frameworks

## Responsibilities

Represent network concepts:

* packets
* TCP connections
* connection states

Main components:

```
Packet.hpp

Connection.hpp

ConnectionKey.hpp

ConnectionState.hpp
```

---

## Packet

`Packet` represents a normalized TCP packet.

It is intentionally different from a raw Ethernet frame.

Example:

```
Raw bytes
    |
    v
Ethernet Header
    |
    v
IPv4 Header
    |
    v
TCP Header
    |
    v
Packet
```

The domain object contains only information required by the application layer.

---

## Connection

A TCP connection is modeled as:

```
ConnectionKey
        +
ConnectionState
        +
Sequence information
```

Example:

```
192.168.1.10:50000
        |
        |
        v

8.8.8.8:443

state = ESTABLISHED
```

---

# Application Layer

Location:

```
src/application/
```

The application layer contains the system behavior.

It coordinates domain objects.

---

## PacketParser

Responsible for converting:

```
uint8_t buffer[]
```

into:

```
ParsedPacket
```

Pipeline:

```
Raw packet
     |
     |
     v
Ethernet validation
     |
     v
IPv4 parsing
     |
     v
TCP parsing
```

Responsibilities:

* validate packet structure
* handle endianness
* reject invalid packets
* extract TCP metadata

It does not:

* open sockets
* store data
* manage connections

---

## PacketMapper

Responsible for converting:

```
ParsedPacket
```

into:

```
Packet
```

This creates a boundary between:

network representation

and

application representation

---

## ConnectionTracker

The connection tracker maintains active TCP sessions.

Internally:

```
unordered_map
<
 ConnectionKey,
 Connection
>
```

Example:

```
ConnectionKey
        |
        v

192.168.1.5:40000
        |
        |
        v
10.0.0.1:443


ConnectionState:
ESTABLISHED
```

Responsibilities:

* create connections
* update states
* remove closed connections
* expose active sessions

---

# State Machine

TCP lifecycle is modeled explicitly.

Current states:

```
CLOSED

SYN_RECEIVED

ESTABLISHED

FIN_WAIT

TIME_WAIT
```

Transitions:

```
        SYN

CLOSED ---------> SYN_RECEIVED


        ACK

SYN_RECEIVED ---> ESTABLISHED


        FIN

ESTABLISHED ----> FIN_WAIT


        ACK

FIN_WAIT --------> CLOSED

```

Invalid transitions are ignored.

---

# Infrastructure Layer

Location:

```
src/infrastructure/
```

Contains external world communication.

Examples:

* Linux raw sockets
* databases
* network injection

---

## RawSocketSniffer

Responsible for packet acquisition.

Implementation:

```
AF_PACKET socket
```

Responsibilities:

* open socket
* bind interface
* receive frames
* deliver callbacks

Flow:

```
Network Interface

        |

        v

RawSocketSniffer

        |

        v

PacketParser

```

The sniffer does not interpret packets.

---

## PacketInjector

Responsible for future active operations.

Possible use cases:

* custom packets
* testing
* controlled experiments

Current status:

Interface prepared.

Implementation evolving.

---

## Persistence

Future components:

```
RedisClient

SQLiteRepository
```

will provide:

Redis:

* active sessions
* realtime counters

SQLite:

* historical data
* archived connections

---

# Dependency Direction

Dependencies always point inward:

```

Infrastructure
       |
       v

Application
       |
       v

Domain

```

The domain never depends on infrastructure.

---

# Testing Architecture

The architecture supports three test levels.

## Unit Tests

Location:

```
tests/unit
```

Cover:

* Packet
* Parser
* ConnectionTracker
* State transitions

No:

* sockets
* databases
* network access

---

## Integration Tests

Location:

```
tests/integration
```

Validate pipelines:

```
Sniffer
   |
Parser
   |
Mapper
   |
Tracker
```

Some tests require:

* root privileges
* raw socket permissions

---

## End To End Tests

Location:

```
tests/e2e
```

Validate:

```
Real network capture

        |

Packet parsing

        |

TCP tracking

        |

Final state

```

---

# Build System

The project uses:

* CMake
* Ninja
* GoogleTest

Build:

```
cmake -S . -B build

cmake --build build
```

Tests:

```
ctest --output-on-failure
```

---

# Future Evolution

The architecture allows adding:

## Observability

Possible additions:

* Prometheus metrics
* structured logs
* tracing

## Storage

Possible additions:

* PostgreSQL backend
* time-series database

## Distributed Mode

Future deployment:

```
Raspberry Pi Agent

        |

        v

Central Monitoring Server

```

---

# Summary

The current architecture provides:

* modular C++ design
* isolated infrastructure
* deterministic testing
* extensibility
* production-oriented structure

The project is intentionally designed as a systems/backend engineering portfolio project rather than a simple packet sniffer.
