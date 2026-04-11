#pragma once
#include <cstdint>

namespace domain {
    struct Packet {
        uint32_t src_ip;
        uint32_t dst_ip;
        uint16_t src_port;
        uint16_t dst_port;
    };
}