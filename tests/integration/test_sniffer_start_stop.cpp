// tests/integration/test_sniffer_start_stop.cpp

#include <gtest/gtest.h>
#include "../../src/infrastructure/RawSocketSniffer.h"

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