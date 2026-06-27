// src/application/PacketMapper.cpp

#include "PacketMapper.hpp"



Packet PacketMapper::map(
    const ParsedPacket& parsed) const
{

    Packet packet{};


    packet.src_ip =
        parsed.ipv4.sourceAddress;


    packet.dst_ip =
        parsed.ipv4.destinationAddress;


    packet.src_port =
        parsed.tcp.sourcePort;


    packet.dst_port =
        parsed.tcp.destinationPort;



    packet.seq =
        parsed.tcp.sequenceNumber;


    packet.ack =
        parsed.tcp.acknowledgementNumber;



    packet.syn =
        parsed.tcp.syn;


    packet.ack_flag =
        parsed.tcp.ack;


    packet.fin =
        parsed.tcp.fin;


    packet.rst =
        parsed.tcp.rst;



    return packet;
}