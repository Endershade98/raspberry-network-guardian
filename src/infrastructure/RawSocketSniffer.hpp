// src/infrastructure/RawSocketSniffer.h

#pragma once

#include "../interfaces/RawSnifferInterface.hpp"

#include <atomic>
#include <string>
#include <thread>

#define BUFFER_SIZE 65536
#define ETH_P_ALL 0x0003
#define IFNAMSIZ 16
#define SIOCGIFINDEX 0x8933

class RawSocketSniffer : public RawSocketSnifferInterface {
public:
    explicit RawSocketSniffer(const std::string& interface);
    ~RawSocketSniffer();

    bool start() override;
    void stop() override;
    void setCallback(PacketCallback cb) override;

private:
    void captureLoop();

    int sockfd{-1};

    std::string interface;
    std::atomic<bool> running{false};
    std::thread worker;

    PacketCallback callback;
};