// src/application/ConnectionTracker.cpp

#include "ConnectionTracker.hpp"

ConnectionKey ConnectionTracker::extractKey(const Packet& packet) const {
    return ConnectionKey{
        packet.src_ip,
        packet.dst_ip,
        packet.src_port,
        packet.dst_port
    };
}

void ConnectionTracker::processPacket(const Packet& packet) {
    ConnectionKey key = extractKey(packet);

    auto it = connections.find(key);

    if (it == connections.end()) {

        if (!(packet.syn && !packet.ack_flag)) {
            return;
        }

        Connection conn;
        conn.key = key;
        conn.state = ConnectionState::CLOSED;
        conn.last_seq = packet.seq;
        conn.last_ack = packet.ack;

        connections[key] = conn;
        it = connections.find(key);
    }

    Connection& conn = it->second;

    conn.state = 
        stateMachine.transition(
            conn.state,
            packet
        );

    conn.last_seq = packet.seq;
    conn.last_ack = packet.ack;

    if (conn.state == ConnectionState::CLOSED) {
        handleClosedConnection(key);
    }
}

void ConnectionTracker::handleClosedConnection(const ConnectionKey& key) {
    connections.erase(key);
}

std::optional<Connection> ConnectionTracker::getConnection(const ConnectionKey& key) const {
    auto it = connections.find(key);

    if (it != connections.end()) {
        return it->second;
    }

    return std::nullopt;
}

std::size_t ConnectionTracker::getActiveConnectionsCount() const {
    return connections.size();
}