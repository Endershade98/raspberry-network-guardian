// tests/unit/test_raw_socket_sniffer.cpp

#include <gtest/gtest.h>
#include "../../src/infrastructure/RawSocketSniffer.h"

TEST(RawSocketSnifferTest, CanConstruct)
{
    RawSocketSniffer sniffer("lo");
    SUCCEED();
}

TEST(RawSocketSnifferTest, CanSetCallback)
{
    RawSocketSniffer sniffer("lo");

    sniffer.setCallback([](const uint8_t*, size_t) {});

    SUCCEED();
}

TEST(RawSocketSnifferTest, StartFailsOnInvalidInterface)
{
    RawSocketSniffer sniffer("fake_interface_xyz");

    bool result = sniffer.start();

    EXPECT_FALSE(result);
}

TEST(RawSocketSnifferTest, StopWithoutStartDoesNotCrash)
{
    RawSocketSniffer sniffer("lo");

    sniffer.stop();

    SUCCEED();
}