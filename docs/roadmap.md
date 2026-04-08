
---

# Epics and Tasks

---

## **Epic 1: Project Setup & Skeleton**
**Goal:** Establish the basic structure, compilation, and testing framework.

**Tasks:**
1. Create folder structure (`src/domain`, `src/application`, `src/infrastructure`, `src/interfaces`, `tests/`).  
2. Set up CMake or Makefile with C++17+ standard.  
3. Add minimal Dockerfile for ARM64 (Raspberry Pi compatible).  
4. Add testing framework (GoogleTest or Catch2).  
5. Write empty unit test stubs for classes (`Packet`, `Connection`, etc.).  

---

## **Epic 2: Raw Packet Capture**
**Goal:** Implement packet sniffing with raw sockets.

**Tasks:**
1. Define interface `IRawSniffer`.  
2. Implement `RawSocketSniffer` in `infrastructure`:  
   - Bind to network interface  
   - Capture loop with buffer  
   - Proper shutdown on SIGINT  
3. Write unit tests for socket creation and buffer handling.  
4. Write integration tests using loopback packets.  
5. Implement minimal CLI to start/stop sniffer.  

---

## **Epic 3: Packet Parsing**
**Goal:** Convert raw bytes to structured packets.

**Tasks:**
1. Define `EthernetHeader`, `IPHeader`, `TCPHeader` structs in `domain/Packet.h`.  
2. Implement `PacketParser` in `application`:  
   - Convert raw buffer → structs  
   - Handle endianness and checksums  
   - Validate packet structure  
3. Unit test parser with synthetic packets (normal and malformed).  
4. Integration test parser with live sniffer output.  

---

## **Epic 4: TCP Connection Tracker**
**Goal:** Track stateful TCP connections.

**Tasks:**
1. Define `ConnectionKey` and `ConnectionState` enum in `domain/Connection.h`.  
2. Implement `ConnectionTracker` in `application`:  
   - Maintain unordered_map of connections  
   - Update state machine per packet (SYN, ACK, FIN, RST)  
3. Unit tests for state transitions.  
4. Integration tests with live/sniffer packets.  

---

## **Epic 5: Packet Injection**
**Goal:** Forge and send custom packets (optional).

**Tasks:**
1. Implement `PacketInjector` in `infrastructure`.  
2. Build IP/TCP headers and calculate checksums manually.  
3. Send packets via raw sockets (`sendto()`).  
4. Unit test header construction and checksum.  
5. E2E tests in lab environment (controlled injection of RST or custom packets).  

---

## **Epic 6: CLI & Logging**
**Goal:** Provide terminal-based interface and structured logging.

**Tasks:**
1. Implement CLI parser (`--interface`, `--verbose`, `--inject`).  
2. Implement logging (INFO, WARN, ERROR) to stdout and file.  
3. Unit tests for CLI and logging functions.  
4. E2E test: run sniffer, parse packets, log states to terminal.  

---

## **Epic 7: Live Data & Persistence**
**Goal:** Integrate Redis and SQL for live and historical data.

**Tasks:**
1. Redis: store active connections, counters, temporary events (TTL).  
2. SQL (SQLite recommended): snapshot closed connections and events periodically.  
3. Unit tests for Redis/SQL access (mocked for unit tests).  
4. Integration/E2E tests: live data update in Redis + periodic archival in SQL.  

---

## **Epic 8: State Machine / Observability**
**Goal:** Monitor connection states and enable intelligent injection.

**Tasks:**
1. Implement minimal TCP state machine in `ConnectionTracker`.  
2. Use enum + update method (CLOSED, SYN_RECEIVED, ESTABLISHED, FIN_WAIT).  
3. Unit tests: state transitions, invalid packets.  
4. Integration tests: live packets → correct state update.  
5. Logging and optional alerts when unusual patterns detected.  

---

## **Epic 9: Testing & CI**
**Goal:** Ensure full coverage.

**Tasks:**
1. Unit tests for domain and application layers.  
2. Integration tests for pipeline: Sniffer → Parser → Tracker → Redis/SQL → CLI.  
3. E2E tests with Docker container (simulated network if needed).  
4. Code coverage metrics with gcov/lcov.  

---

## **Epic 10: Docker & Portability**
**Goal:** Make project fully portable on ARM64 / Raspberry Pi.

**Tasks:**
1. Dockerfile for building and running `network_guardian`.  
2. Environment variable configuration (`INTERFACE`, `LOG_LEVEL`, etc.).  
3. Volumes for SQL DB and Redis persistence.  
4. Test container end-to-end: sniffer running, packets captured, states logged.  
5. Optional: docker-compose for running network_guardian + Redis + SQLite.  

