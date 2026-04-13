// src/infrastructure/RawSocketSniffer.h
#pragma once

#include "../interfaces/RawSnifferInterface.h"
#include <atomic>
#include <string>
#include <thread>

using namespace interfaces;
using namespace infrastructure;

#define BUFFER_SIZE 65536
#define ETH_P_ALL 0x0003
#define IFNAMSIZ 16
#define SIOCGIFINDEX 0x8933
#define IS_RUNNING false

namespace infrastructure {
    
    class RawSocketSniffer : public RawSocketSnifferInterface {
        public:
            explicit RawSocketSniffer(const string& interface);
            ~RawSocketSniffer();

            bool start() override;
            void stop() override;
            void setCallback(PacketCallback cb) override;

        private:
            void captureLoop();

            int sockfd;
            string interface;
            atomic<bool> running;
            thread worker;

            PacketCallback callback;
        };
}




