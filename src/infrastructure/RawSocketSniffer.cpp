// src/infrastructure/RawSocketSniffer.cpp

#include "RawSocketSniffer.hpp"

#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/time.h>
#include <cerrno>

using namespace std;

RawSocketSniffer::RawSocketSniffer(const std::string& iface)
    : sockfd(-1),
      interface(iface),
      running(false) {}

RawSocketSniffer::~RawSocketSniffer() {
    stop();
}

bool RawSocketSniffer::start() {
    if (running) {
        return true;
    }

    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if (sockfd < 0) {
        perror("socket");
        return false;
    }

    // Professional solution:
    // make recvfrom() wake up periodically so stop() is responsive
    timeval timeout{};
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    if (setsockopt(sockfd,
                   SOL_SOCKET,
                   SO_RCVTIMEO,
                   &timeout,
                   sizeof(timeout)) < 0) {
        perror("setsockopt");
        close(sockfd);
        sockfd = -1;
        return false;
    }

    struct ifreq ifr{};
    std::strncpy(ifr.ifr_name, interface.c_str(), IFNAMSIZ - 1);
    ifr.ifr_name[IFNAMSIZ - 1] = '\0';

    if (ioctl(sockfd, SIOCGIFINDEX, &ifr) < 0) {
        perror("ioctl");
        close(sockfd);
        sockfd = -1;
        return false;
    }

    sockaddr_ll sll{};
    sll.sll_family   = AF_PACKET;
    sll.sll_ifindex  = ifr.ifr_ifindex;
    sll.sll_protocol = htons(ETH_P_ALL);

    if (bind(sockfd,
             reinterpret_cast<sockaddr*>(&sll),
             sizeof(sll)) < 0) {
        perror("bind");
        close(sockfd);
        sockfd = -1;
        return false;
    }

    running = true;
    worker = std::thread(&RawSocketSniffer::captureLoop, this);

    return true;
}

void RawSocketSniffer::stop() {
    running = false;

    if (worker.joinable()) {
        worker.join();
    }

    if (sockfd != -1) {
        close(sockfd);
        sockfd = -1;
    }
}

void RawSocketSniffer::captureLoop() {
    uint8_t buffer[BUFFER_SIZE];

    while (running) {
        ssize_t len = recvfrom(
            sockfd,
            buffer,
            sizeof(buffer),
            0,
            nullptr,
            nullptr
        );

        if (!running) {
            break;
        }

        // timeout or interrupted syscall
        if (len < 0) {
            if (errno == EAGAIN ||
                errno == EWOULDBLOCK ||
                errno == EINTR) {
                continue;
            }

            // socket closed or fatal error
            break;
        }

        if (len == 0) {
            continue;
        }

        if (callback) {
            callback(buffer, static_cast<size_t>(len));
        }
    }
}

void RawSocketSniffer::setCallback(PacketCallback cb) {
    callback = std::move(cb);
}