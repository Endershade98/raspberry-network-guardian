// include/network_guardian/application/PacketMapper.hpp

#pragma once

#include <network_guardian/domain/Packet.hpp>


class PacketMapper
{
public:

    Packet map(
        const ParsedPacket& parsed
    ) const;

};