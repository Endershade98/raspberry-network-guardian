// tests/unit/test_connection_tracker.cpp

#include <gtest/gtest.h>

#include "../../src/application/PacketParser.hpp"

#include <vector>


class PacketParserTest : public ::testing::Test
{
protected:

    PacketParser parser;


    std::vector<uint8_t> buildPacket(
        uint8_t version = 4,
        uint8_t ihl = 5,
        uint8_t protocol = 6,
        uint8_t flags = 0x02)
    {

        std::vector<uint8_t> buffer(
            14 + (ihl * 4) + 20,
            0
        );


        // ethernet ipv4

        buffer[12] = 0x08;
        buffer[13] = 0x00;


        // ipv4

        buffer[14] =
            (version << 4) | ihl;


        buffer[23] =
            protocol;


        // tcp flags

        auto tcp =
            14 + ihl * 4;


        buffer[tcp + 13] =
            flags;


        return buffer;
    }
};



TEST_F(PacketParserTest, RejectsShortEthernet)
{
    uint8_t buffer[5]{};


    auto result =
        parser.parse(buffer,5);


    EXPECT_FALSE(result.has_value());
}



TEST_F(PacketParserTest, ParsesValidIPv4TCP)
{
    auto raw =
        buildPacket();


    auto result =
        parser.parse(
            raw.data(),
            raw.size());


    EXPECT_TRUE(result.has_value());
}



TEST_F(PacketParserTest, RejectsIPv6)
{
    auto raw =
        buildPacket(6);


    EXPECT_FALSE(
        parser.parse(
            raw.data(),
            raw.size())
        .has_value());
}



TEST_F(PacketParserTest, RejectsInvalidIHL)
{
    auto raw =
        buildPacket(4,4);


    EXPECT_FALSE(
        parser.parse(
            raw.data(),
            raw.size())
        .has_value());
}



TEST_F(PacketParserTest, RejectsUDP)
{
    auto raw =
        buildPacket(4,5,17);


    EXPECT_FALSE(
        parser.parse(
            raw.data(),
            raw.size())
        .has_value());
}



TEST_F(PacketParserTest, ParsesSYN)
{
    auto raw =
        buildPacket(4,5,6,0x02);


    auto packet =
        parser.parse(
            raw.data(),
            raw.size());


    ASSERT_TRUE(packet);


    EXPECT_TRUE(packet->tcp.syn);
}



TEST_F(PacketParserTest, ParsesACK)
{
    auto raw =
        buildPacket(4,5,6,0x10);


    auto packet =
        parser.parse(
            raw.data(),
            raw.size());


    ASSERT_TRUE(packet);


    EXPECT_TRUE(packet->tcp.ack);
}



TEST_F(PacketParserTest, ParsesFIN)
{
    auto raw =
        buildPacket(4,5,6,0x01);


    auto packet =
        parser.parse(
            raw.data(),
            raw.size());


    ASSERT_TRUE(packet);


    EXPECT_TRUE(packet->tcp.fin);
}



TEST_F(PacketParserTest, ParsesRST)
{
    auto raw =
        buildPacket(4,5,6,0x04);


    auto packet =
        parser.parse(
            raw.data(),
            raw.size());


    ASSERT_TRUE(packet);


    EXPECT_TRUE(packet->tcp.rst);
}