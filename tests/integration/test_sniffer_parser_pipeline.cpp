// tests/integration/test_sniffer_parser_pipeline.cpp

#include <gtest/gtest.h>


#include <network_guardian/infrastructure/RawSocketSniffer.hpp>
#include <network_guardian/application/PacketParser.hpp>
#include <network_guardian/application/PacketMapper.hpp>
#include <network_guardian/application/ConnectionTracker.hpp>


#include <atomic>
#include <thread>
#include <chrono>
#include <cstdlib>



TEST(
    PipelineIntegrationTest,
    SnifferProducesPackets
)
{

    RawSocketSniffer sniffer("lo");


    PacketParser parser;

    PacketMapper mapper;

    ConnectionTracker tracker;



    std::atomic<bool> captured(false);



    sniffer.setCallback(
        [&](const uint8_t* data, size_t size)
        {

            if(data == nullptr)
                return;


            if(size == 0)
                return;



            captured = true;


            /*
             * Nota:
             *
             * Parser viene testato
             * separatamente.
             *
             * Lo sniffer non deve
             * conoscere il formato
             * del datalink.
             */


        }
    );



    if(!sniffer.start())
    {
        GTEST_SKIP()
        << "Need CAP_NET_RAW";
    }



    std::system(
        "ping -c 1 127.0.0.1 >/dev/null 2>&1"
    );



    std::this_thread::sleep_for(
        std::chrono::seconds(1)
    );



    sniffer.stop();



    EXPECT_TRUE(captured);
}