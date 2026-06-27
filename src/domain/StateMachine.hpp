// src/domain/StateMachine.hpp

#pragma once

#include "ConnectionState.hpp"
#include "Packet.hpp"

class StateMachine
{
public:

    ConnectionState transition(
        ConnectionState current,
        const Packet& packet
    ) const;
};