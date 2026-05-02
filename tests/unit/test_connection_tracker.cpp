// tests/unit/test_connection_tracker.cpp

#include <gtest/gtest.h>
#include "../../src/application/ConnectionTracker.h"

TEST(ConnectionTrackerTest, CreatesNewConnectionOnSYN) {
    ConnectionTracker tracker;

    Packet syn{1,2,1000,80,0,0,true,false,false,false};

    tracker.processPacket(syn);

    ConnectionKey key{1,2,1000,80};

    auto conn = tracker.getConnection(key);

    ASSERT_TRUE(conn.has_value());
    EXPECT_EQ(conn->state, ConnectionState::SYN_RECEIVED);
}

TEST(ConnectionTrackerTest, CompletesHandshake) {
    ConnectionTracker tracker;

    Packet syn{1,2,1000,80,0,0,true,false,false,false};
    Packet ack{1,2,1000,80,1,1,false,true,false,false};

    tracker.processPacket(syn);
    tracker.processPacket(ack);

    auto conn = tracker.getConnection({1,2,1000,80});

    ASSERT_TRUE(conn.has_value());
    EXPECT_EQ(conn->state, ConnectionState::ESTABLISHED);
}

TEST(ConnectionTrackerTest, ClosesConnectionOnRST) {
    ConnectionTracker tracker;

    Packet syn{1,2,1000,80,0,0,true,false,false,false};
    Packet rst{1,2,1000,80,0,0,false,false,false,true};

    tracker.processPacket(syn);
    tracker.processPacket(rst);

    auto conn = tracker.getConnection({1,2,1000,80});

    EXPECT_FALSE(conn.has_value());
}

TEST(ConnectionTrackerTest, TracksMultipleConnections) {
    ConnectionTracker tracker;

    Packet a{1,2,1000,80,0,0,true,false,false,false};
    Packet b{3,4,2000,443,0,0,true,false,false,false};

    tracker.processPacket(a);
    tracker.processPacket(b);

    EXPECT_EQ(tracker.getActiveConnectionsCount(), 2u);
}

TEST(ConnectionTrackerTest, IgnoresInvalidAckAsFirstPacket) {
    ConnectionTracker tracker;

    Packet ack{1,2,1000,80,1,1,false,true,false,false};

    tracker.processPacket(ack);

    EXPECT_EQ(tracker.getActiveConnectionsCount(), 0u);
}