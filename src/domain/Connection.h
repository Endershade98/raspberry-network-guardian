// src/domain/Connection.h
#pragma once
#include "ConnectionKey.h"
#include "ConnectionState.h"

using namespace std;
using namespace domain;
namespace domain {
    struct Connection {
        ConnectionKey key;
        ConnectionState state;

        uint32_t last_seq;
        uint32_t last_ack;
    };
};