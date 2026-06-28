// include/network_guardian/infrastructure/PacketInjector.hpp

#pragma once

#include <string>
#include<stdlib.h>

#include <network_guardian/interfaces/RawSnifferInterface.hpp>


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
