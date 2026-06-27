// src/domain/ConnectionState.h

#pragma once

enum class ConnectionState {
    CLOSED,
    SYN_RECEIVED,
    ESTABLISHED,
    FIN_WAIT,
    TIME_WAIT
};