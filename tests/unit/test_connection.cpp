// tests/unit/test_connection.cpp
#include <cassert>
#include "../../src/domain/Connection.h"
using namespace domain;

void test_connection_initial_state() {
    Connection conn{0, 0, 0, 0, ConnectionState::CLOSED};
    assert(conn.state == ConnectionState::CLOSED);
}

void test_connection_syn_received() {
    Connection conn{0, 0, 0, 0, ConnectionState::SYN_RECEIVED};
    assert(conn.state == ConnectionState::SYN_RECEIVED);
}

void test_connection_established() {
    Connection conn{0, 0, 0, 0, ConnectionState::ESTABLISHED};
    assert(conn.state == ConnectionState::ESTABLISHED);
}

void test_connection_fin_wait() {
    Connection conn{0, 0, 0, 0, ConnectionState::FIN_WAIT};
    assert(conn.state == ConnectionState::FIN_WAIT);
}

