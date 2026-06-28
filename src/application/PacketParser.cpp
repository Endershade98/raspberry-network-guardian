// src/application/PacketParser.cpp

#include <arpa/inet.h>

#include <cstring>
#include <network_guardian/application/PacketParser.hpp>


namespace
{
    constexpr std::size_t ETHERNET_HEADER_SIZE = 14;
    constexpr std::size_t IPV4_MIN_HEADER_SIZE = 20;
    constexpr std::size_t TCP_MIN_HEADER_SIZE = 20;

    constexpr uint16_t ETHERTYPE_IPV4 = 0x0800;
    constexpr uint8_t TCP_PROTOCOL = 6;
}


std::optional<ParsedPacket>
PacketParser::parse(
    const uint8_t* buffer,
    std::size_t length
) const
{
    if(buffer == nullptr)
        return std::nullopt;


    ParsedPacket packet{};


    if(!parseEthernet(
        buffer,
        length,
        packet.ethernet))
    {
        return std::nullopt;
    }


    std::size_t tcpOffset = 0;


    if(!parseIPv4(
        buffer,
        length,
        packet.ipv4,
        tcpOffset))
    {
        return std::nullopt;
    }


    if(!parseTCP(
        buffer,
        length,
        tcpOffset,
        packet.tcp))
    {
        return std::nullopt;
    }


    return packet;
}



bool PacketParser::parseEthernet(
    const uint8_t* buffer,
    std::size_t length,
    EthernetHeader& header
) const
{
    if(length < ETHERNET_HEADER_SIZE)
        return false;


    std::memcpy(
        header.destination,
        buffer,
        6);


    std::memcpy(
        header.source,
        buffer + 6,
        6);


    header.etherType =
        ntohs(
            *reinterpret_cast<
            const uint16_t*>(buffer + 12)
        );


    return header.etherType == ETHERTYPE_IPV4;
}



bool PacketParser::parseIPv4(
    const uint8_t* buffer,
    std::size_t length,
    IPv4Header& header,
    std::size_t& tcpOffset
) const
{

    if(length <
       ETHERNET_HEADER_SIZE +
       IPV4_MIN_HEADER_SIZE)
    {
        return false;
    }



    const uint8_t* ip =
        buffer + ETHERNET_HEADER_SIZE;



    header.version =
        ip[0] >> 4;


    header.ihl =
        ip[0] & 0x0F;



    if(header.version != 4)
        return false;



    if(header.ihl < 5)
        return false;



    std::size_t ipHeaderSize =
        header.ihl * 4;



    if(length <
       ETHERNET_HEADER_SIZE +
       ipHeaderSize)
    {
        return false;
    }



    header.totalLength =
        ntohs(
            *reinterpret_cast<
            const uint16_t*>(ip + 2)
        );



    header.protocol =
        ip[9];



    // IMPORTANTISSIMO:
    // blocca UDP e qualsiasi altro protocollo

    if(header.protocol != TCP_PROTOCOL)
        return false;



    header.sourceAddress =
        ntohl(
            *reinterpret_cast<
            const uint32_t*>(ip + 12)
        );



    header.destinationAddress =
        ntohl(
            *reinterpret_cast<
            const uint32_t*>(ip + 16)
        );



    tcpOffset =
        ETHERNET_HEADER_SIZE +
        ipHeaderSize;



    return true;
}



bool PacketParser::parseTCP(
    const uint8_t* buffer,
    std::size_t length,
    std::size_t offset,
    TCPHeader& header
) const
{

    if(length <
       offset + TCP_MIN_HEADER_SIZE)
    {
        return false;
    }



    const uint8_t* tcp =
        buffer + offset;



    header.sourcePort =
        ntohs(
            *reinterpret_cast<
            const uint16_t*>(tcp)
        );



    header.destinationPort =
        ntohs(
            *reinterpret_cast<
            const uint16_t*>(tcp + 2)
        );



    header.sequenceNumber =
        ntohl(
            *reinterpret_cast<
            const uint32_t*>(tcp + 4)
        );



    header.acknowledgementNumber =
        ntohl(
            *reinterpret_cast<
            const uint32_t*>(tcp + 8)
        );



    uint8_t flags =
        tcp[13];


    header.fin =
        flags & 0x01;


    header.syn =
        flags & 0x02;


    header.rst =
        flags & 0x04;


    header.ack =
        flags & 0x10;



    return true;
}