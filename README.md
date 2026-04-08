# Raspberry Pi Network Guardian

Raspberry Pi Network Guardian is a low-level network monitoring and packet manipulation tool written in C++.  
It captures raw packets on a network interface, parses Ethernet/IP/TCP headers, tracks TCP connection states, and optionally injects custom packets.  

Key features:
- Raw socket packet capture
- TCP connection tracking with state machine
- Minimal CLI-based interface, no GUI
- Redis for live data and counters
- SQLite for historical traffic and statistics
- Fully testable (unit, integration, e2e)
- Dockerized for portability on Raspberry Pi or other ARM64 Linux systems

## Build & Run
```bash
# Build with Docker
docker build -t network_guardian .

# Run container with privileged access for raw sockets
docker run --net=host --privileged network_guardian --interface eth0