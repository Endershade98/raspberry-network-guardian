# Project Roadmap

## Raspberry Network Guardian

This roadmap tracks the evolution of the project from a low-level TCP monitoring prototype into a production-quality C++ networking observability platform.

The current priority is not only adding features, but improving:

- software architecture
- maintainability
- testing strategy
- portability
- documentation
- production readiness

---

# Current Status

## Completed

### Epic 1 — Project Setup & Skeleton ✅

Goal:
Establish a maintainable C++ project foundation.

Completed:

- C++17 project structure
- CMake build system
- Ninja support
- GoogleTest integration
- Unit/integration/E2E test targets
- Domain/Application/Infrastructure separation

Current structure:

```

src/
├── domain
├── application
├── infrastructure
└── interfaces

tests/
├── unit
├── integration
└── e2e

```

---

# Epic 2 — Raw Packet Capture ✅

Goal:

Capture network traffic using Linux raw sockets.

Completed:

- Raw socket implementation
- Interface binding
- Packet receive loop
- Threaded capture execution
- Graceful shutdown
- Callback based packet delivery

Implemented:

```

infrastructure/

RawSocketSniffer

```

Features:

- AF_PACKET socket
- Ethernet frame capture
- interface selection
- start/stop lifecycle management

Testing:

Unit:

- construction
- callback registration
- invalid socket handling

Integration:

- loopback capture tests

E2E:

- real interface capture

---

# Epic 3 — Packet Parsing ✅

Goal:

Convert raw network frames into structured domain packets.

Completed:

Implemented:

```

application/

PacketParser

```

Supported parsing:

- Ethernet
- IPv4
- TCP

Capabilities:

- header validation
- protocol filtering
- endian conversion
- TCP flag extraction


Pipeline:

```

Raw bytes

```
↓
```

EthernetHeader

```
↓
```

IPv4Header

```
↓
```

TCPHeader

```
↓
```

ParsedPacket

```

Testing:

Implemented tests for:

- invalid ethernet frames
- invalid IPv4 packets
- unsupported protocols
- TCP SYN
- TCP ACK
- TCP FIN
- TCP RST

---

# Epic 4 — TCP Connection Tracker ✅

Goal:

Track TCP connections and lifecycle.

Completed:

Implemented:

```

application/

ConnectionTracker

```

Features:

- connection identification
- active connection storage
- TCP state transitions
- connection removal

Domain model:

```

Connection

ConnectionKey

ConnectionState

```

Current states:

```

CLOSED

SYN_RECEIVED

ESTABLISHED

FIN_WAIT

TIME_WAIT

```

Supported transitions:

```

SYN

CLOSED
|
v

SYN_RECEIVED

ACK

SYN_RECEIVED
|
v

ESTABLISHED

FIN

ESTABLISHED
|
v

FIN_WAIT

RST / FIN ACK

FIN_WAIT
|
v

CLOSED

```

Testing:

Unit:

- connection creation
- handshake tracking
- reset handling
- state transitions

Integration:

- parser → mapper → tracker pipeline

E2E:

- live capture
- parse
- track flow

---

# Current Project Maturity

The project currently provides:

```

Network Interface

```
    |
    v
```

RawSocketSniffer

```
    |
    v
```

PacketParser

```
    |
    v
```

PacketMapper

```
    |
    v
```

ConnectionTracker

```
    |
    v
```

Connection State Model

```

This represents a functional network monitoring core.

---

# Next Priority: Production Readiness Phase

Before implementing new features, the project will enter a hardening phase.

---

# Epic 5 — Code Quality & Production Readiness

Goal:

Transform the prototype into a portfolio-grade engineering project.

Tasks:

## Build System

- improve CMake organization
- add install target
- add export targets
- add compiler presets
- add clang-format
- add clang-tidy


Deliver:

```

cmake/
├── warnings.cmake
├── sanitizers.cmake
└── options.cmake

````

---

## Static Analysis

Add:

- clang-format
- clang-tidy
- cppcheck

Goals:

- remove warnings
- enforce style
- improve maintainability


---

## Error Handling

Replace:

- boolean failures
- perror usage
- silent failures


With:

- typed errors
- exceptions where appropriate
- structured error reporting


Example:

Before:

```cpp
bool start();
````

After:

```cpp
Result<void, SnifferError> start();
```

---

## Logging System

Implement:

```
interfaces/

Logger
```

Features:

* log levels
* timestamps
* file output
* structured messages

Example:

```
[INFO]
Sniffer started on eth0


[WARN]
Invalid TCP packet received


[ERROR]
Unable to bind socket
```

---

# Epic 6 — Testing & CI

Goal:

Provide professional automated verification.

Tasks:

## Unit Testing

Expand coverage:

* domain objects
* state machine
* parser edge cases

Target:

```
>80% coverage
```

---

## Integration Testing

Complete:

```
Sniffer

↓

Parser

↓

Mapper

↓

Tracker

↓

Persistence
```

---

## Continuous Integration

Add:

GitHub Actions:

Pipeline:

```
push

↓

build

↓

test

↓

coverage

↓

static analysis
```

---

# Epic 7 — Observability Layer

Goal:

Expose network state.

Tasks:

Implement:

* connection counters
* traffic statistics
* events

Example:

```
Active connections: 42

TCP SYN received: 1200

RST packets: 15
```

---

# Epic 8 — Persistence

Goal:

Store network history.

Components:

Redis:

Real-time state

```
active connections
counters
TTL events
```

SQLite:

Historical data:

```
closed connections

network events

statistics
```

---

# Epic 9 — CLI Interface

Goal:

Create production user interface.

Commands:

Example:

```
network_guardian start

network_guardian status

network_guardian connections

network_guardian stop
```

Options:

```
--interface eth0

--verbose

--output json
```

---

# Epic 10 — Docker & Raspberry Pi Deployment

Goal:

Run on embedded hardware.

Tasks:

Docker:

* multi-stage build
* ARM64 support
* minimal runtime image

Compose:

Services:

```
guardian

redis

database
```

Target:

Raspberry Pi deployment.

---

# Long Term Vision

The final architecture should evolve into:

```
                CLI

                 |

        Network Guardian Core

                 |

        +----------------+

        |                |

    Sniffer          Storage

        |

    Parser

        |

    Analyzer

        |

    Connection Engine

        |

    Observability
```

The final goal is a modular C++ network monitoring platform suitable for:

* embedded Linux
* Raspberry Pi
* backend systems
* network observability environments
