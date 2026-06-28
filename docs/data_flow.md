# Data Flow Documentation

## Overview

This document describes the runtime data flow inside **Raspberry Network Guardian**.

The system is designed as a layered packet processing pipeline:

```

Network Interface
|
v
RawSocketSniffer
|
v
PacketParser
|
v
PacketMapper
|
v
ConnectionTracker
|
v
Connection State Model

```

The goal of the pipeline is to transform low-level network traffic into
high-level TCP connection state information.

---

# 1. Packet Capture Flow

## Component

`src/infrastructure/RawSocketSniffer`

Responsibility:

- Open Linux raw socket
- Bind socket to network interface
- Receive raw Ethernet frames
- Forward captured data to upper layers


## Input

The sniffer receives raw bytes:

```

uint8_t buffer[]

```

Example:

```

+----------------+
| Ethernet Frame |
+----------------+
| IPv4 Header    |
+----------------+
| TCP Header     |
+----------------+
| Payload        |
+----------------+

```

The sniffer does not interpret packet contents.

It only guarantees:

```

raw bytes -> callback

```


## Runtime flow

```

socket(AF_PACKET)
|
v
bind(interface)
|
v
recvfrom()
|
v
callback(buffer, size)

```

---

# 2. Packet Parsing Flow

## Component

`src/application/PacketParser`

Responsibility:

Convert raw network bytes into domain structures.


Input:

```

uint8_t*
size_t

```

Output:

```

ParsedPacket

```


The parser executes three phases:

```

Ethernet Parsing
|
v
IPv4 Parsing
|
v
TCP Parsing

```


---

# 2.1 Ethernet Parsing

The parser validates:

- minimum frame size
- Ethernet type


Example:

```

Ethernet Header

Destination MAC
Source MAC
EtherType

```


Only IPv4 traffic is accepted.


Flow:

```

Raw Frame

|
v

EthernetHeader

|
|
+---- not IPv4 ---> discard

|
v

continue

```


---

# 2.2 IPv4 Parsing

The parser extracts:

```

source IP
destination IP
protocol
header size

```


Validation:

```

IPv4 ?
correct header length ?
TCP protocol ?

```


Invalid packets are rejected early.


Example:

```

IPv4
|
+-- UDP  -> ignore
|
+-- TCP  -> continue

```


---

# 2.3 TCP Parsing

TCP information extracted:

```

source port
destination port

sequence number
ack number

flags:
SYN
ACK
FIN
RST

```


Result:

```

ParsedPacket
{
EthernetHeader
IPv4Header
TCPHeader
}

```


---

# 3. Packet Mapping Flow

## Component

`PacketMapper`

Responsibility:

Convert parser representation into application domain model.


Why?

The parser works with network concepts.

The tracker works with connection concepts.


Boundary:

```

Infrastructure representation

```
    |

    v
```

Application domain model

```


Transformation:


Before:

```

TCPHeader

sourcePort
destinationPort
sequenceNumber
flags

```


After:

```

Packet

src_ip
dst_ip
src_port
dst_port

seq
ack

syn
ack_flag
fin
rst

```


The tracker does not know about Ethernet or TCP headers.


---

# 4. Connection Tracking Flow

## Component

`ConnectionTracker`

Responsibility:

Maintain active TCP connections.


Internal storage:


```

unordered_map

ConnectionKey
|
v
Connection

```


Example:


```

192.168.1.10:50000
|
|
v

192.168.1.20:443

```
    |
    v
```

ESTABLISHED

```


---

# 4.1 Connection Creation


A connection starts when:


```

SYN == true
ACK == false

```


Example:


Client:

```

SYN
------>
Server

```


Tracker creates:

```

ConnectionState::CLOSED

```


Then transitions:

```

CLOSED
|
v
SYN_RECEIVED

```


---

# 4.2 TCP State Progression


Current implementation:


```

```
          SYN
          
```

CLOSED ----------------> SYN_RECEIVED

SYN_RECEIVED
|
|
ACK
|
v

ESTABLISHED

ESTABLISHED
|
FIN
|
v

FIN_WAIT

FIN_WAIT
|
ACK
|
v

CLOSED

```


RST handling:


```

any state

|
v

CLOSED

```


---

# 5. End-to-End Runtime Example


A browser opens HTTPS connection.


## Step 1

Kernel receives packet:

```

Ethernet
IPv4
TCP SYN

```


## Step 2

RawSocketSniffer captures:

```

bytes[74]

```


## Step 3

PacketParser creates:


```

ParsedPacket

src:
192.168.1.5

dst:
142.250.x.x

tcp:
SYN=true
ACK=false

```


## Step 4

PacketMapper creates:


```

Packet

192.168.1.5:54000
|
|
v
142.250.x.x:443

```


## Step 5

Tracker updates:


```

ConnectionState

CLOSED
|
v
SYN_RECEIVED

```


---

# 6. Error Handling Strategy


Each layer rejects invalid data as early as possible.


Example:


```

RawSocketSniffer
|
|
v

PacketParser

invalid packet?
|
+---- discard

valid packet
|
v

ConnectionTracker

```


This prevents corrupted network data from reaching business logic.


---

# 7. Design Principles


## Separation of concerns


Capture layer:

```

"What bytes arrived?"

```


Parser layer:

```

"What does this packet contain?"

```


Application layer:

```

"What does this mean?"

```


Domain layer:

```

"What is the current network state?"

```


---

# Future Extensions


The pipeline is designed to support:


```

RawSocketSniffer
|
v
PacketParser
|
v
PacketMapper
|
+------------+
|            |
v            v
ConnectionTracker   EventLogger

```
    |
    v
```

Redis / SQLite Persistence

```


Future additions:

- connection metrics
- traffic statistics
- anomaly detection
- packet injection
- observability 