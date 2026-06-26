// tests/integration/test_sniffer_parser.cpp

#include <gtest/gtest.h>

#include "../../src/application/PacketParser.hpp"
#include "../../src/infrastructure/RawSocketSniffer.hpp"

#include <atomic>
#include <thread>

TEST(SnifferParserIntegration, CanCaptureAndParse)
{
    RawSocketSniffer sniffer("lo");

    PacketParser parser;

    std::atomic<bool> parsed(false);

    sniffer.setCallback(
        [&](const uint8_t* data,size_t len)
        {
            auto packet =
                parser.parse(data,len);

            if(packet)
                parsed = true;
        });

    if(!sniffer.start())
        GTEST_SKIP();

    std::system(
        "ping -c 1 127.0.0.1 >/dev/null 2>&1");

    std::this_thread::sleep_for(
        std::chrono::seconds(1));

    sniffer.stop();

    EXPECT_TRUE(parsed);
}