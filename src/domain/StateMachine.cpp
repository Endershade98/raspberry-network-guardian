// src/domain/StateMachine.cpp

#include <network_guardian/domain/StateMachine.hpp>
#include <network_guardian/domain/Packet.hpp>


ConnectionState StateMachine::transition(
    ConnectionState current,
    const Packet& packet) const
{
    if (packet.rst)
    {
        return ConnectionState::CLOSED;
    }

    switch (current)
    {
        case ConnectionState::CLOSED:

            if (packet.syn && !packet.ack_flag)
            {
                return ConnectionState::SYN_RECEIVED;
            }

            break;

        case ConnectionState::SYN_RECEIVED:

            if (packet.ack_flag)
            {
                return ConnectionState::ESTABLISHED;
            }

            break;

        case ConnectionState::ESTABLISHED:

            if (packet.fin)
            {
                return ConnectionState::FIN_WAIT;
            }

            break;

        case ConnectionState::FIN_WAIT:

            if (packet.ack_flag)
            {
                return ConnectionState::CLOSED;
            }

            break;

        case ConnectionState::TIME_WAIT:
            return ConnectionState::CLOSED;
    }

    return current;
}