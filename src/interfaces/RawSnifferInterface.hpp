// src/interfaces/RawSnifferInterface.hpp

#pragma once

#include <functional>
#include <cstdint>

class RawSocketSnifferInterface {
    public:
        using PacketCallback = std::function<void(const uint8_t*, size_t)>;

        virtual ~RawSocketSnifferInterface() = default;

        virtual bool start() = 0;
        virtual void stop() = 0;
        
        virtual void setCallback(PacketCallback cb) = 0;
};