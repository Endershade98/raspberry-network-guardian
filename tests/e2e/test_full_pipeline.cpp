// tests/e2e/test_full_pipeline.cpp

#include <gtest/gtest.h>

#include "../../src/infrastructure/RawSocketSniffer.hpp"
#include "../../src/application/PacketParser.hpp"
#include "../../src/application/PacketMapper.hpp"
#include "../../src/application/ConnectionTracker.hpp"

#include <atomic>
#include <thread>
#include <chrono>
#include <cstdlib>


TEST(E2E, CaptureParseTrackFlow)
{

    RawSocketSniffer sniffer("lo");

    PacketParser parser;
    PacketMapper mapper;
    ConnectionTracker tracker;


    std::atomic<bool> processed=false;


    sniffer.setCallback(
        [&](const uint8_t* data,size_t len)
        {

            auto parsed =
                parser.parse(
                    data,
                    len);


            if(!parsed)
                return;


            Packet packet =
                mapper.map(
                    *parsed);


            tracker.processPacket(packet);


            processed=true;
        });



    if(!sniffer.start())
    {
        GTEST_SKIP()
        << "requires root";
    }


    //
    // genera traffico TCP reale
    //
    std::system(
        "curl http://127.0.0.1:80 >/dev/null 2>&1 || true"
    );


    std::this_thread::sleep_for(
        std::chrono::seconds(2));


    sniffer.stop();


    EXPECT_TRUE(processed);
}