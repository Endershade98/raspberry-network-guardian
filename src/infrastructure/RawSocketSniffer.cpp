// src/infrastructure/RawSocketSniffer.cpp
#include "RawSocketSniffer.h"

#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <netinet/in.h>

using namespace std;
using namespace interfaces;
using namespace infrastructure;

RawSocketSniffer::RawSocketSniffer(const string& iface)
    : interface(iface), running(IS_RUNNING), sockfd(-1) {}

RawSocketSniffer::~RawSocketSniffer() {
    stop();
}

bool RawSocketSniffer::start() {
    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if (sockfd < 0) {
        perror("socket");
        return false;
    }

    struct ifreq ifr{};
    strncpy(ifr.ifr_name, interface.c_str(), IFNAMSIZ);

    if (ioctl(sockfd, SIOCGIFINDEX, &ifr) < 0) {
        perror("ioctl");
        return false;
    }

    struct sockaddr_ll sll{};
    sll.sll_family = AF_PACKET;
    sll.sll_ifindex = ifr.ifr_ifindex;
    sll.sll_protocol = htons(ETH_P_ALL);

    if (bind(sockfd, (struct sockaddr*)&sll, sizeof(sll)) < 0) {
        perror("bind");
        return false;
    }

    running = !IS_RUNNING;
    worker = std::thread(&RawSocketSniffer::captureLoop, this);

    return true;
}

void RawSocketSniffer::stop() {
    running = IS_RUNNING;

    if (sockfd != -1) {
        close(sockfd);
        sockfd = -1;
    }

    if (worker.joinable()) {
        worker.join();
    }
}

void RawSocketSniffer::captureLoop() {
    uint8_t buffer[BUFFER_SIZE];

    while (IS_RUNNING == running) {
        ssize_t len = recvfrom(sockfd, buffer, sizeof(buffer), 0, nullptr, nullptr);

        if (len <= 0) continue;

        if (callback) {
            callback(buffer, static_cast<size_t>(len));
        }
    }
}

void RawSocketSniffer::setCallback(PacketCallback cb) {
    callback = cb;
}
