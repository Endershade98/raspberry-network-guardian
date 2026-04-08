# Flowcharts

## **Flow 1: Packet Capture → Parsing → Tracking**

```text
RawSocketSniffer
      │ (raw bytes)
      ▼
PacketParser
      │ (ParsedPacket: Ethernet/IP/TCP)
      ▼
ConnectionTracker
      │ (update TCP state machine)
      ▼
Redis (live connections & counters)
      │
      ▼
SQLite (historical snapshots, optional)
      │
      ▼
CLI / Logger
      │
      ▼
PacketInjector (optional)
      │
      ▼
Network
```

---

## **Flow 2: Packet Injection Decision**


```text
ConnectionTracker
      │ (state + policy)
      ▼
PacketInjector
      │ (forge IP/TCP headers)
      ▼
sendto() → Network
```

---

### **Flow 3: Observability & Logging**


```text
ConnectionTracker
      │ (state, counters, events)
      ▼
Redis (TTL live data)
      │
      ▼
CLI / Logger (terminal output)
      │
      ▼
Optional SQLite snapshot (archival)
```