// tests/unit/test_connection.cpp

#include <gtest/gtest.h>
#include "../../src/domain/Connection.hpp"

TEST(ConnectionTest, DefaultClosedState) {
    Connection conn{};

    EXPECT_EQ(conn.state, ConnectionState::CLOSED);
}

TEST(ConnectionTest, EstablishedState) {
    Connection conn{
        {1,2,1000,80},
        ConnectionState::ESTABLISHED,
        10,
        20
    };

    EXPECT_EQ(conn.state, ConnectionState::ESTABLISHED);
    EXPECT_EQ(conn.last_seq, 10u);
    EXPECT_EQ(conn.last_ack, 20u);
}