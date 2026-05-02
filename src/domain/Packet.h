// src/domain/Packet.h

#pragma once

#include <cstdint>

struct Packet {
    uint32_t src_ip{0};
    uint32_t dst_ip{0};

    uint16_t src_port{0};
    uint16_t dst_port{0};

    uint32_t seq{0};
    uint32_t ack{0};

    bool syn{false};
    bool ack_flag{false};
    bool fin{false};
    bool rst{false};
};