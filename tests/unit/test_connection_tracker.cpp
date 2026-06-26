// tests/unit/test_connection_tracker.cpp

#include <gtest/gtest.h>

#include "../../src/application/ConnectionTracker.hpp"


TEST(ConnectionTrackerTest, StartsEmpty)
{
    ConnectionTracker tracker;


    EXPECT_EQ(
        tracker.getActiveConnectionsCount(),
        0);
}



TEST(ConnectionTrackerTest, CreatesConnectionOnSyn)
{
    ConnectionTracker tracker;


    Packet packet{};

    packet.src_ip=1;
    packet.dst_ip=2;
    packet.src_port=1000;
    packet.dst_port=80;
    packet.syn=true;


    tracker.processPacket(packet);


    auto result =
        tracker.getConnection(
            {1,2,1000,80});


    ASSERT_TRUE(result);


    EXPECT_EQ(
        result->state,
        ConnectionState::SYN_RECEIVED);
}



TEST(ConnectionTrackerTest, EstablishesHandshake)
{
    ConnectionTracker tracker;


    Packet syn{};
    syn.src_ip=1;
    syn.dst_ip=2;
    syn.src_port=1000;
    syn.dst_port=80;
    syn.syn=true;


    Packet ack=syn;

    ack.syn=false;
    ack.ack_flag=true;



    tracker.processPacket(syn);
    tracker.processPacket(ack);


    auto c =
        tracker.getConnection(
            {1,2,1000,80});


    ASSERT_TRUE(c);


    EXPECT_EQ(
        c->state,
        ConnectionState::ESTABLISHED);
}



TEST(ConnectionTrackerTest, RemovesRST)
{
    ConnectionTracker tracker;


    Packet syn{};

    syn.src_ip=1;
    syn.dst_ip=2;
    syn.src_port=1000;
    syn.dst_port=80;
    syn.syn=true;


    Packet rst=syn;

    rst.syn=false;
    rst.rst=true;



    tracker.processPacket(syn);
    tracker.processPacket(rst);



    EXPECT_EQ(
        tracker.getActiveConnectionsCount(),
        0);
}