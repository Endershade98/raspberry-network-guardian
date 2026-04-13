// src/infrastructure/PacketInjector.h
#pragma once
#include "../interfaces/RawSnifferInterface.h"
#include <string>

using namespace std;
using namespace interfaces;

namespace infrastructure {
    class PacketInjector {
    public:
        explicit PacketInjector(const string& interface);
        ~PacketInjector();

        bool inject(const uint8_t* data, size_t len);

    private:
        int sockfd;
        string interface;
    };
}