// include/network_guardian/domain/ConnectionState.hpp

#pragma once


enum class ConnectionState {
    CLOSED,
    SYN_RECEIVED,
    ESTABLISHED,
    FIN_WAIT,
    TIME_WAIT
};