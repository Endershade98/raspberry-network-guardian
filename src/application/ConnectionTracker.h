// src/application/ConnectionTracker.h
#pragma once

#include <unordered_map>
#include <cstdint>
#include <optional>
#include "../domain/Connection.h"
#include "../domain/ConnectionKey.h"
#include "../domain/ConnectionState.h"
#include "../domain/Packet.h"

using namespace domain;
using namespace std;

namespace application {
    class ConnectionTracker {
    public:
        ConnectionTracker();

        void processPacket(const Packet& packet);

        std::optional<Connection> getConnection(const ConnectionKey& key) const;

        size_t getActiveConnectionsCount() const;

    private:
        std::unordered_map<ConnectionKey, Connection, ConnectionKeyHash> connections;

        ConnectionKey extractKey(const Packet& packet) const;

        void updateState(Connection& conn, const Packet& packet);

        void handleClosedConnection(const ConnectionKey& key);
    };
}