## TCP State Machine (Markdown Table)

### **Table: TCP Connection States**

| Code / Enum          | Description                                    | Trigger / Event                      | Action / Notes                                |
|----------------------|-----------------------------------------------|-------------------------------------|-----------------------------------------------|
| CLOSED               | No active connection                           | Initial state / FIN complete        | Connection removed from tracker               |
| SYN_RECEIVED         | SYN received, waiting for ACK                  | Packet: SYN                          | Update sequence numbers                       |
| ESTABLISHED          | Connection established                         | Packet: ACK after SYN               | Normal data transfer                           |
| FIN_WAIT             | FIN received, waiting for ACK                  | Packet: FIN                          | Begin termination, still tracked              |
| RST                  | Connection reset                               | Packet: RST                          | Remove connection immediately                 |
| UNKNOWN / INVALID    | Packet cannot be mapped to any state          | Malformed packet / unexpected flag  | Log and ignore, optionally alert              |

---

## Redis / SQL Role Table

| Component | Type      | Data Stored                         | TTL / Persistence | Purpose                                      |
|-----------|----------|------------------------------------|-----------------|---------------------------------------------|
| Redis     | In-Memory | Active connections, counters, short-lived events | Yes (TTL for temporary data) | Live monitoring, minimal memory usage       |
| SQLite    | Persistent | Historical connections, traffic stats, snapshots | Permanent        | Archive for reporting, long-term analysis |

---

## Packet Data Flow Table

| Module                | Input                | Output                  | Responsibility                               |
|-----------------------|--------------------|------------------------|---------------------------------------------|
| RawSocketSniffer      | Network interface  | Raw bytes buffer       | Capture raw packets from NIC                |
| PacketParser          | Raw bytes          | ParsedPacket (structs) | Convert bytes → Ethernet/IP/TCP headers    |
| ConnectionTracker     | ParsedPacket       | ConnectionState update | Track TCP state, update counters, decide injection |
| PacketInjector        | ConnectionState / CLI | Raw packet sent       | Forge and send TCP/IP packets               |
| CLI / Logger          | ConnectionState / counters | Terminal output    | Display live state, logs, alerts            |
| Redis                 | ConnectionState / counters | In-memory snapshot | Live observability, TTL events              |
| SQLite                | Periodic snapshot  | Persistent storage     | Historical data for analysis                |

---

## TCP State Machine Diagram (Textual Flow)

```text
CLOSED
  │ SYN received
  ▼
SYN_RECEIVED
  │ ACK received
  ▼
ESTABLISHED
  │ FIN received
  ▼
FIN_WAIT
  │ ACK sent
  ▼
CLOSED
```

---

## Packet Lifecycle Diagram

```text
[Network] → RawSocketSniffer → PacketParser → ConnectionTracker → {Redis, SQLite, CLI/Logger} → Optional PacketInjector → [Network]
```