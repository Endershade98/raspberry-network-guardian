// tests/unit/test_packet.cpp

#include <gtest/gtest.h>
#include "../../src/domain/Packet.h"

TEST(PacketTest, DefaultConstruction) {
    Packet packet{};

    EXPECT_EQ(packet.src_ip, 0u);
    EXPECT_FALSE(packet.syn);
}
