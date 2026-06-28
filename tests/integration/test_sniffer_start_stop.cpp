// tests/integration/test_sniffer_start_stop.cpp

#include <gtest/gtest.h>

#include <network_guardian/infrastructure/RawSocketSniffer.hpp>


TEST(RawSocketSnifferIntegrationTest, StartAndStopRealSocket)
{
    RawSocketSniffer sniffer("lo");

    if (!sniffer.start())
    {
        GTEST_SKIP() << "Need root privileges";
    }

    sniffer.stop();

    SUCCEED();
}