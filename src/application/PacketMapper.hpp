// src/application/PacketMapper.hpp

#pragma once


#include "../domain/Packet.hpp"



class PacketMapper
{
public:

    Packet map(
        const ParsedPacket& parsed
    ) const;

};