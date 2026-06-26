// tests/unit/test_packet_parser.cpp

#include <gtest/gtest.h>

#include "../../src/application/PacketParser.hpp"

#include <vector>


class ParserTest :
public testing::Test
{

protected:

PacketParser parser;



std::vector<uint8_t>
validPacket(
uint8_t version=4,
uint8_t ihl=5,
uint8_t proto=6,
uint8_t flags=2)
{

std::vector<uint8_t>
p(54,0);


p[12]=0x08;
p[13]=0x00;


p[14]=(version<<4)|ihl;


p[23]=proto;


p[34+12]=0x50;
p[34+13]=flags;


return p;
}

};



TEST_F(ParserTest,EthernetTooShort)
{

uint8_t data[5]{};


EXPECT_FALSE(
parser.parse(data,5)
.has_value());

}



TEST_F(ParserTest,ValidIPv4)
{

auto p=validPacket();


EXPECT_TRUE(
parser.parse(
p.data(),
p.size())
.has_value());

}



TEST_F(ParserTest,RejectVersion)
{

auto p=validPacket(6);


EXPECT_FALSE(
parser.parse(
p.data(),
p.size())
.has_value());

}



TEST_F(ParserTest,RejectIHL)
{

auto p=validPacket(4,4);


EXPECT_FALSE(
parser.parse(
p.data(),
p.size())
.has_value());

}



TEST_F(ParserTest,RejectUDP)
{

auto p=validPacket(4,5,17);


EXPECT_FALSE(
parser.parse(
p.data(),
p.size())
.has_value());

}



TEST_F(ParserTest,SYN)
{

auto p=validPacket(4,5,6,2);


auto r=
parser.parse(
p.data(),
p.size());


ASSERT_TRUE(r);


EXPECT_TRUE(
r->tcp.syn);

}