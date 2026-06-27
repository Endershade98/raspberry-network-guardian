// include/network_guardian/domain/Packet.hpp

#pragma once

#include <cstdint>


struct Packet
{
    uint32_t src_ip{0};
    uint32_t dst_ip{0};

    uint16_t src_port{0};
    uint16_t dst_port{0};


    uint32_t seq{0};
    uint32_t ack{0};


    bool syn{false};
    bool ack_flag{false};
    bool fin{false};
    bool rst{false};
};

struct EthernetHeader
{
    uint8_t destination[6];
    uint8_t source[6];

    uint16_t etherType;
};

struct IPv4Header
{
    uint8_t version;
    uint8_t ihl;

    uint8_t protocol;

    uint16_t totalLength;

    uint32_t sourceAddress;
    uint32_t destinationAddress;
};

struct TCPHeader
{
    uint16_t sourcePort;
    uint16_t destinationPort;

    uint32_t sequenceNumber;
    uint32_t acknowledgementNumber;

    bool syn;
    bool ack;
    bool fin;
    bool rst;
};

struct ParsedPacket
{
    EthernetHeader ethernet;
    IPv4Header ipv4;
    TCPHeader tcp;
};