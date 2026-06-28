// tests/e2e/test_full_pipeline.cpp

#include <gtest/gtest.h>

#include <network_guardian/application/ConnectionTracker.hpp>
#include <network_guardian/application/PacketMapper.hpp>
#include <network_guardian/application/PacketParser.hpp>
#include <network_guardian/infrastructure/RawSocketSniffer.hpp>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <atomic>
#include <chrono>
#include <thread>

TEST(E2E, CaptureParseTrackFlow)
{
    RawSocketSniffer sniffer("lo");

    PacketParser parser;
    PacketMapper mapper;
    ConnectionTracker tracker;

    std::atomic<bool> processed{false};

    sniffer.setCallback(
        [&](const uint8_t* data, size_t len)
        {
            auto parsed = parser.parse(data, len);

            if (!parsed)
                return;

            Packet packet = mapper.map(*parsed);

            tracker.processPacket(packet);

            processed = true;
        });

    if (!sniffer.start())
    {
        GTEST_SKIP() << "Raw sockets require root privileges.";
    }

    constexpr uint16_t PORT = 45678;

    std::thread server([]
    {
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        ASSERT_NE(server_fd, -1);

        int opt = 1;
        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(PORT);
        addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

        ASSERT_EQ(bind(
                      server_fd,
                      reinterpret_cast<sockaddr*>(&addr),
                      sizeof(addr)),
                  0);

        ASSERT_EQ(listen(server_fd, 1), 0);

        int client =
            accept(server_fd, nullptr, nullptr);

        if (client >= 0)
        {
            close(client);
        }

        close(server_fd);
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    int client_fd = socket(AF_INET, SOCK_STREAM, 0);

    ASSERT_NE(client_fd, -1);

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    inet_pton(
        AF_INET,
        "127.0.0.1",
        &server_addr.sin_addr);

    ASSERT_EQ(
        connect(
            client_fd,
            reinterpret_cast<sockaddr*>(&server_addr),
            sizeof(server_addr)),
        0);

    close(client_fd);

    server.join();

    using namespace std::chrono_literals;

    const auto timeout =
        std::chrono::steady_clock::now() + 2s;

    while (!processed &&
           std::chrono::steady_clock::now() < timeout)
    {
        std::this_thread::sleep_for(10ms);
    }

    sniffer.stop();

    EXPECT_TRUE(processed);
}