// tests/integration/test_sniffer_loopback.cpp

#include <gtest/gtest.h>
#include "../../src/infrastructure/RawSocketSniffer.hpp"

#include <thread>
#include <chrono>
#include <atomic>
#include <cstdlib>

TEST(RawSocketSnifferIntegrationTest, CaptureLoopbackTraffic)
{
    RawSocketSniffer sniffer("lo");

    std::atomic<bool> packetReceived = false;

    sniffer.setCallback([&](const uint8_t*, size_t)
    {
        packetReceived = true;
    });

    if (!sniffer.start())
    {
        GTEST_SKIP() << "Raw socket requires sudo/root";
    }

    std::system("ping -c 1 127.0.0.1 > /dev/null 2>&1");

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    sniffer.stop();

    EXPECT_TRUE(packetReceived);
}