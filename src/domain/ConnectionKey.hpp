// src/domain/ConnectionKey.h

#pragma once

#include <cstdint>
#include <functional>

struct ConnectionKey {
    uint32_t src_ip{0};
    uint32_t dst_ip{0};

    uint16_t src_port{0};
    uint16_t dst_port{0};

    bool operator==(const ConnectionKey& other) const {
        return src_ip == other.src_ip &&
               dst_ip == other.dst_ip &&
               src_port == other.src_port &&
               dst_port == other.dst_port;
    }
};

struct ConnectionKeyHash {
    std::size_t operator()(const ConnectionKey& k) const {
        return std::hash<uint64_t>()(
            (static_cast<uint64_t>(k.src_ip) << 32) | k.dst_ip
        ) ^
        std::hash<uint32_t>()(
            (static_cast<uint32_t>(k.src_port) << 16) | k.dst_port
        );
    }
};