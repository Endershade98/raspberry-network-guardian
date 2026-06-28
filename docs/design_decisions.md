# Design Decisions

## Overview

This document describes the main architectural and engineering decisions
taken during the development of **Raspberry Network Guardian**.

The goal is to document the reasoning behind the design choices,
trade-offs, and future evolution paths.

---

# 1. Layered Architecture

## Decision

The project follows a layered architecture inspired by Clean Architecture principles.

The system is divided into:


```

+-----------------------------+
| Interfaces                  |
| CLI / Logging               |
+-----------------------------+

```
        |
        v
```

+-----------------------------+
| Application                 |
| Parser / Tracker / Mapping  |
+-----------------------------+

```
        |
        v
```

+-----------------------------+
| Domain                      |
| Packet / Connection Models  |
+-----------------------------+

```
        ^
        |
        |
```

+-----------------------------+
| Infrastructure              |
| Raw Socket / Database       |
+-----------------------------+

```


## Reasoning

Network applications often mix:

- operating system APIs
- packet formats
- business logic
- persistence

Mixing these responsibilities creates tightly coupled code.

Separating layers provides:

- easier testing
- replaceable infrastructure
- clearer ownership of responsibilities


---

# 2. Domain Models Are Independent

## Decision

The domain layer contains pure C++ structures.

Examples:

```

Packet
Connection
ConnectionKey
ConnectionState

```


These structures do not depend on:

- Linux socket APIs
- GoogleTest
- Redis
- SQLite


## Reasoning

The domain represents the core business meaning.

A TCP connection exists independently from how packets are captured.

This allows future changes such as:

- replaying packets from files
- loading data from databases
- simulated testing


---

# 3. Raw Socket Capture Isolation

## Decision

Raw packet capture is isolated inside:

```

src/infrastructure/RawSocketSniffer

```


The rest of the system receives only:

```

callback(buffer, size)

```


## Reasoning

Raw sockets are:

- Linux specific
- permission restricted
- difficult to test


Keeping them isolated prevents the entire application from depending on
kernel APIs.


Future alternatives:

- libpcap backend
- AF_XDP backend
- offline PCAP reader


can replace the sniffer without rewriting application logic.


---

# 4. Parser Does Not Create Application Objects

## Decision

PacketParser produces:

```

ParsedPacket

```


not:

```

Packet

```


## Reasoning

The parser responsibility is decoding.

It answers:

"what fields exist inside these bytes?"


The application layer answers:

"what does this information mean?"


This separation prevents network protocol details leaking into the business layer.


Flow:


```

bytes

|

v

ParsedPacket

|

v

Packet

|

v

Connection

```


---

# 5. PacketMapper as Explicit Boundary

## Decision

A dedicated mapper converts:

```

ParsedPacket

```

into:


```

Packet

```


## Reasoning

Without a mapper:

```

ConnectionTracker
|
|
v

TCPHeader
IPv4Header
EthernetHeader

```


The tracker would become coupled to packet formats.


With the mapper:


```

Parser

|

v

Application Model

|

v

Tracker

```


The tracker only understands connections.


---

# 6. TCP State Machine Representation

## Decision

TCP states are represented using:

```

enum class ConnectionState

```


Current states:

```

CLOSED
SYN_RECEIVED
ESTABLISHED
FIN_WAIT
TIME_WAIT

```


## Reasoning

Using enums provides:


- compiler validation
- explicit transitions
- readable tests


Example:


```

if(state == ESTABLISHED)

```


is preferable to:


```

if(state == 2)

```


---

# 7. Connection Tracking Using Hash Map

## Decision

Active connections are stored in:

```

unordered_map<ConnectionKey, Connection>

```


## Reasoning


TCP connections require fast lookup.

The key:

```

src_ip
dst_ip
src_port
dst_port

```


uniquely identifies a flow.


Expected complexity:

```

insert: O(1)
lookup: O(1)
remove: O(1)

```


This fits the expected workload of a network monitor.


---

# 8. Test Strategy

## Decision

Testing is divided into:


```

Unit Tests

Integration Tests

End-to-End Tests

```


## Unit Tests

Validate isolated components:

Examples:

- PacketParser
- ConnectionTracker
- Packet models


No real network required.


---

## Integration Tests

Validate component interaction:


```

Sniffer

|

Parser

|

Tracker

```


These may require:

- root permissions
- real interfaces


---

## End-to-End Tests

Validate complete behaviour:


```

Network traffic

```
  |

  v
```

Capture

```
  |

  v
```

Parse

```
  |

  v
```

Track connection

```


---

# 9. GoogleTest Choice

## Decision

The project uses GoogleTest.


## Reasoning

Advantages:

- industry adoption
- excellent CMake integration
- assertions
- test discovery


It also mirrors testing practices commonly found in large C++ codebases.


---

# 10. CMake Build System

## Decision

The project uses CMake.

Minimum version:

```

3.20

```


## Reasoning

CMake provides:

- cross-platform builds
- dependency management
- IDE integration
- CI compatibility


The project can build on:

- Linux
- Raspberry Pi OS
- Docker containers


---

# 11. Error Handling Strategy

## Decision

Invalid data is rejected as early as possible.


Example:


```

Raw Packet

|

v

Parser

|

invalid

discard

```


## Reasoning

Network input cannot be trusted.


Failing early:

- protects later layers
- simplifies logic
- avoids undefined behaviour


---

# 12. Performance Considerations

## Current design goals

The first priority is correctness and maintainability.


Optimizations are postponed until profiling shows a need.


Potential future optimizations:


## Zero-copy parsing

Avoid copying buffers.


## Memory pools

Reuse packet objects.


## Lock-free queues

Separate capture thread from processing thread.


---

# 13. Future Evolution

The current architecture intentionally supports:


```

RawSocket
|
v
Packet Processing Pipeline
|
+------ Redis
|
+------ SQLite
|
+------ Metrics
|
+------ Alerts

```


Future modules can be added without changing the core pipeline.


---

# Summary

The project prioritizes:

- separation of responsibilities
- testability
- portability
- maintainable C++ design
- explicit boundaries


The architecture is designed to evolve from a learning project into
a production-style network monitoring system.