# Testing Strategy

## Overview

Raspberry Network Guardian uses a layered testing strategy designed to validate
correctness from isolated components up to the complete network monitoring flow.

The testing philosophy follows three levels:

```

```
      End-to-End Tests

            ▲

      Integration Tests

            ▲

        Unit Tests
```

```

Each level has a specific purpose and trade-off.

---

# 1. Testing Goals

The test suite aims to guarantee:

- correctness of packet processing
- reliability of TCP state tracking
- safe handling of malformed network input
- predictable behaviour under real network traffic
- portability across development environments


The project prioritizes:

1. deterministic tests
2. fast feedback
3. isolated failures
4. reproducible execution


---

# 2. Test Framework

The project uses:

```

GoogleTest

```

Version:

```

1.14.0

```

Integration is handled through CMake:

```

FetchContent
GoogleTest
CTest

```

Advantages:

- industry standard C++ testing framework
- native CMake support
- automatic test discovery
- rich assertion system

---

# 3. Test Layout

The tests are organized following the production architecture.

```

tests/

├── unit/
│
├── integration/
│
└── e2e/

```

---

# 4. Unit Tests

## Purpose

Unit tests validate individual components without depending on:

- real network interfaces
- root permissions
- external services


They should be:

- fast
- deterministic
- isolated


---

## Covered Components

### Domain Layer

Tested objects:

```

Packet
Connection
ConnectionKey
ConnectionState

```

Examples:

- default initialization
- equality
- hashing
- state values


---

### Packet Parser

Tests validate:


```

Raw bytes

```
  |

  v
```

ParsedPacket

```


Covered scenarios:

## Valid packets

Examples:

- valid Ethernet frame
- valid IPv4 header
- valid TCP header


## Invalid packets


Examples:

- too short buffer
- unsupported protocol
- invalid IP version
- invalid header length


---

## Connection Tracker

The tracker is tested as a TCP state machine.


Example flow:


```

SYN

|

v

SYN_RECEIVED

|

ACK

|

v

ESTABLISHED

```


Other transitions:

```

RST

|

v

CLOSED

```


Tests verify:

- connection creation
- state transitions
- connection removal
- active connection counting


---

# 5. Integration Tests

## Purpose

Integration tests validate collaboration between modules.


Pipeline tested:


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


---

# Network Dependency

Some integration tests require:

- Linux raw sockets
- CAP_NET_RAW capability
- root privileges


Example:

```

sudo ./build/integration_tests

```


Without permissions:

expected behaviour:

```

SKIPPED

```


This is intentional.

The test is not failing;
the environment does not provide the required capability.


---

# Integration Test Categories


## Sniffer Tests

Validate:

- socket creation
- interface binding
- start/stop lifecycle
- callback execution


Example:


```

RawSocketSniffer

start()

capture packet

callback()

stop()

```


---

## Parser Pipeline Tests


Validate:


```

Captured Packet

```
   |

   v
```

ParsedPacket

```
   |

   v
```

Connection

```


The goal is verifying that layers communicate correctly.


---

# 6. End-to-End Tests

## Purpose

E2E tests validate the complete application behaviour.


They simulate a realistic execution:


```

Network Traffic

```
    |

    v
```

Raw Capture

```
    |

    v
```

Parsing

```
    |

    v
```

Mapping

```
    |

    v
```

TCP Tracking

```


---

# Current E2E Scenario


Test:

```

CaptureParseTrackFlow

```


Validates:


1. Start sniffer

2. Capture traffic

3. Parse packets

4. Convert packet model

5. Update TCP state

6. Verify tracked connection


Expected:

```

PASS

```


---

# 7. Running Tests

## Build

```

just build

```


---

## Unit Tests

```

just unit

```


Expected:

```

[ PASSED ]

```


---

## Integration Tests

```

just integration

```


For raw socket tests:


```

sudo ./build/integration_tests

```


---

## End-to-End Tests

```

just e2e

```


Requires:

```

CAP_NET_RAW

```

or root execution.


---

# 8. Test Philosophy


## Prefer Realistic Tests

Whenever possible:

- use real packet structures
- use realistic TCP flows
- validate complete behaviour


Avoid:

- excessive mocking
- testing implementation details


---

# 9. CI Strategy

The project is designed to support CI pipelines.


Recommended stages:


```

Stage 1

Compile

```
    |
```

Stage 2

Run Unit Tests

```
    |
```

Stage 3

Run Integration Tests

```
    |
```

Stage 4

Run E2E Tests

```


---

# 10. Coverage


Coverage support is available through:


```

gcov
lcov
genhtml

```


Enable:


```

just coverage

```


Produces:


```

coverage-html/

```


---

# 11. Future Testing Improvements


## Additional Unit Coverage

Future targets:

- Redis client
- SQLite repository
- CLI parser
- Logger


---

## Property Based Testing

Potential future addition:

Generate random packets and verify:

- parser never crashes
- invalid input is rejected safely


---

## Packet Replay Testing


Add PCAP based testing:


```

.pcap file

|

v

Parser

|

v

Tracker

```


Benefits:

- deterministic network scenarios
- regression testing
- debugging real traffic


---

# 12. Quality Gates


Before merging changes:


Required:

```

Build successful

*

Unit tests passing

*

Integration tests passing

*

E2E passing when environment allows

```


---

# Summary

The testing system ensures that Raspberry Network Guardian remains:

- reliable
- maintainable
- portable
- safe against malformed network input


The current test suite validates the complete processing pipeline
from raw network traffic to tracked TCP connections.