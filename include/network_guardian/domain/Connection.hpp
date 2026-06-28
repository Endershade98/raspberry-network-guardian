// include/network_guardian/domain/Connection.hpp

#pragma once

#include <cstdint>

#include <network_guardian/domain/ConnectionKey.hpp>
#include <network_guardian/domain/ConnectionState.hpp>


struct Connection {
    ConnectionKey key{};
    ConnectionState state{ConnectionState::CLOSED};

    uint32_t last_seq{0};
    uint32_t last_ack{0};
};