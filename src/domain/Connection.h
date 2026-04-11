#pragma once
#include <cstdint>

namespace domain {
    enum class ConnectionState {
        CLOSED = 0,
        SYN_RECEIVED,
        ESTABLISHED,
        FIN_WAIT
    };

    struct Connection {
        uint32_t src_ip;
        uint32_t dst_ip;
        uint16_t src_port;
        uint16_t dst_port;
        ConnectionState state;
    };
}