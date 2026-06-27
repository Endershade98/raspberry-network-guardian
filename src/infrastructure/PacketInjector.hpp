// src/infrastructure/PacketInjector.h

#pragma once

#include "../interfaces/RawSnifferInterface.hpp"

#include <string>
#include<stdlib.h>

using namespace std;


class PacketInjector {
    public:
        explicit PacketInjector(const string& interface);
        ~PacketInjector();

        bool inject(const uint8_t* data, size_t len);

    private:
        int sockfd;
        string interface;
};
