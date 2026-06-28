// tests/integration/test_sniffer_parser.cpp

#include <gtest/gtest.h>

#include <network_guardian/infrastructure/RawSocketSniffer.hpp>
#include <network_guardian/application/PacketParser.hpp>

#include <atomic>
#include <thread>
#include <chrono>
#include <cstdlib>


TEST(
    SnifferParserIntegration,
    CanCaptureTraffic
)
{

    RawSocketSniffer sniffer("lo");


    PacketParser parser;


    std::atomic<bool> received(false);



    sniffer.setCallback(
        [&](const uint8_t* data, size_t len)
        {

            if(data == nullptr)
                return;


            if(len == 0)
                return;


            /*
             * Questo test verifica
             * solamente che il layer
             * raw socket riceva dati.
             *
             * Il parsing è già coperto
             * dai test unitari.
             */

            received = true;
        }
    );



    if(!sniffer.start())
    {
        GTEST_SKIP()
        << "Raw socket requires CAP_NET_RAW";
    }



    std::system(
        "ping -c 1 127.0.0.1 >/dev/null 2>&1"
    );



    std::this_thread::sleep_for(
        std::chrono::seconds(1)
    );



    sniffer.stop();



    EXPECT_TRUE(received);
}