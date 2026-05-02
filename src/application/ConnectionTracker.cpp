// src/application/ConnectionTracker.cpp

#include "ConnectionTracker.h"

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

    updateState(conn, packet);

    conn.last_seq = packet.seq;
    conn.last_ack = packet.ack;

    if (conn.state == ConnectionState::CLOSED) {
        handleClosedConnection(key);
    }
}

void ConnectionTracker::updateState(Connection& conn, const Packet& packet) {

    if (packet.rst) {
        conn.state = ConnectionState::CLOSED;
        return;
    }

    switch (conn.state) {

        case ConnectionState::CLOSED:
            if (packet.syn && !packet.ack_flag)
                conn.state = ConnectionState::SYN_RECEIVED;
            break;

        case ConnectionState::SYN_RECEIVED:
            if (packet.ack_flag)
                conn.state = ConnectionState::ESTABLISHED;
            break;

        case ConnectionState::ESTABLISHED:
            if (packet.fin)
                conn.state = ConnectionState::FIN_WAIT;
            break;

        case ConnectionState::FIN_WAIT:
            if (packet.ack_flag)
                conn.state = ConnectionState::CLOSED;
            break;
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