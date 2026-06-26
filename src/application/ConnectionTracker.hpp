// src/application/ConnectionTracker.hpp

#pragma once

#include <unordered_map>
#include <optional>
#include <cstddef>


#include "../domain/Connection.hpp"
#include "../domain/Packet.hpp"


class ConnectionTracker
{

public:

    void processPacket(
        const Packet& packet);


    std::optional<Connection>
    getConnection(
        const ConnectionKey&) const;



    std::size_t
    getActiveConnectionsCount() const;



private:


    std::unordered_map<
        ConnectionKey,
        Connection,
        ConnectionKeyHash
    > connections;



    ConnectionKey extractKey(
        const Packet&) const;



    void updateState(
        Connection&,
        const Packet&);



    void handleClosedConnection(
        const ConnectionKey&);
};