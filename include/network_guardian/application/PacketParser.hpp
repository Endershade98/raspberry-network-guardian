// include/network_guardian/application/PacketParser.hpp

#pragma once

#include <optional>
#include <cstddef>
#include <cstdint>

#include <network_guardian/domain/Packet.hpp>


class PacketParser
{
public:

    std::optional<ParsedPacket> parse(
        const uint8_t* buffer,
        std::size_t length) const;

private:

    bool parseEthernet(
        const uint8_t* buffer,
        std::size_t length,
        EthernetHeader& header) const;

    bool parseIPv4(
        const uint8_t* buffer,
        std::size_t length,
        IPv4Header& header,
        std::size_t& headerOffset) const;

    bool parseTCP(
        const uint8_t* buffer,
        std::size_t length,
        std::size_t offset,
        TCPHeader& header) const;
};