// src/domain/Connection.h
#pragma once
#include <cstdint>

namespace domain {
    enum class ConnectionState {
        CLOSED = 0,
        SYN_RECEIVED,
        ESTABLISHED,
        FIN_WAIT
    };
    enum class ConnectionEvent {
        SYN,
        ACK,
        FIN,
        TIMEOUT,
        NONE
    };
    class ConnectionKey {
        /*
        Class to uniquely identify a connection based on source/destination IP and ports. 
        Used as a key in connection tracking maps.
        */
    public:
        ConnectionKey(uint32_t src_ip, uint32_t dst_ip, uint16_t src_port, uint16_t dst_port)
            : src_ip(src_ip), dst_ip(dst_ip), src_port(src_port), dst_port(dst_port) {}

        bool operator==(const ConnectionKey& other) const {
            return src_ip == other.src_ip && dst_ip == other.dst_ip &&
                   src_port == other.src_port && dst_port == other.dst_port;
        }

        uint32_t src_ip;
        uint32_t dst_ip;
        uint16_t src_port;
        uint16_t dst_port;
    };

    struct Connection {
        uint32_t src_ip;
        uint32_t dst_ip;
        uint16_t src_port;
        uint16_t dst_port;
        ConnectionState state;
    };
}