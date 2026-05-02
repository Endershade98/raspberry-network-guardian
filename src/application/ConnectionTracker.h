// src/application/ConnectionTracker.h

#pragma once

#include <unordered_map>
#include <optional>
#include <cstddef>

#include "../domain/Connection.h"
#include "../domain/Packet.h"

class ConnectionTracker {
public:
    ConnectionTracker() = default;

    void processPacket(const Packet& packet);

    std::optional<Connection> getConnection(const ConnectionKey& key) const;

    std::size_t getActiveConnectionsCount() const;

private:
    std::unordered_map<ConnectionKey, Connection, ConnectionKeyHash> connections;

    ConnectionKey extractKey(const Packet& packet) const;

    void updateState(Connection& conn, const Packet& packet);

    void handleClosedConnection(const ConnectionKey& key);
};