// tests/integration/test_parser_mapper_tracker.cpp

#include <gtest/gtest.h>

#include <network_guardian/application/PacketParser.hpp>
#include <network_guardian/application/PacketMapper.hpp>
#include <network_guardian/application/ConnectionTracker.hpp>


TEST(Pipeline,
ParserToTracker)
{


PacketParser parser;

PacketMapper mapper;

ConnectionTracker tracker;



uint8_t raw[54]{};


raw[12]=0x08;
raw[13]=0x00;

raw[14]=0x45;

raw[23]=6;


raw[34+12]=0x50;
raw[34+13]=0x02;



auto parsed =
parser.parse(
raw,
sizeof(raw));



ASSERT_TRUE(parsed);



Packet packet =
mapper.map(*parsed);



tracker.processPacket(packet);



EXPECT_EQ(
tracker.getActiveConnectionsCount(),
1);

}