// src/domain/ConnectionState.h

#pragma once

enum class ConnectionState {
    CLOSED = 0,
    SYN_RECEIVED,
    ESTABLISHED,
    FIN_WAIT
};