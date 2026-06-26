// tests/integration/test_sniffer_parser_pipeline.cpp

#include <gtest/gtest.h>


#include "../../src/infrastructure/RawSocketSniffer.hpp"
#include "../../src/application/PacketParser.hpp"


#include <atomic>
#include <thread>



TEST(PipelineIntegrationTest,
     SnifferProducesParsablePackets)
{

    RawSocketSniffer sniffer("lo");


    PacketParser parser;


    std::atomic<bool> parsed=false;



    sniffer.setCallback(
        [&](const uint8_t* data,size_t size)
        {

            auto packet =
                parser.parse(
                    data,
                    size);


            if(packet)
                parsed=true;
        });



    if(!sniffer.start())
    {
        GTEST_SKIP()
        << "Need root";
    }



    system(
      "ping -c 1 127.0.0.1 >/dev/null");


    std::this_thread::sleep_for(
        std::chrono::seconds(1));


    sniffer.stop();


    EXPECT_TRUE(parsed);
}