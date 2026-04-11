#include <cassert>
#include "../../src/domain/Connection.h"
using namespace domain;

void test_connection_initial_state() {
    Connection conn{0, 0, 0, 0, ConnectionState::CLOSED};
    assert(conn.state == ConnectionState::CLOSED);
}

int main() {
    test_connection_initial_state();
    return 0;
}