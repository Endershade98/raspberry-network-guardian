// tests/unit/test_packet.cpp

#include <gtest/gtest.h>

#include "../../src/domain/Packet.hpp"


TEST(PacketTest, DefaultPacket)
{

    Packet packet{};


    EXPECT_EQ(packet.src_ip,0);
    EXPECT_FALSE(packet.syn);

}