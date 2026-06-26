// src/domain/Connection.h

#pragma once

#include <cstdint>

#include "ConnectionKey.hpp"
#include "ConnectionState.hpp"

struct Connection {
    ConnectionKey key{};
    ConnectionState state{ConnectionState::CLOSED};

    uint32_t last_seq{0};
    uint32_t last_ack{0};
};