// include/network_guardian/domain/StateMachine.hpp

#pragma once

#include <network_guardian/domain/ConnectionState.hpp>
#include <network_guardian/domain/Packet.hpp>


class StateMachine
{
public:

    ConnectionState transition(
        ConnectionState current,
        const Packet& packet
    ) const;
};