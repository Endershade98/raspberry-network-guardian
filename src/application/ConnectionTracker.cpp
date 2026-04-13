// src/application/ConnectionTracker.cpp
#include "ConnectionTracker.h"
using namespace domain;
using namespace std;
using namespace application;

ConnectionTracker::ConnectionTracker() {}

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
        // Nuova connessione
        Connection conn;
        conn.key = key;
        conn.state = ConnectionState::CLOSED;
        conn.last_seq = packet.seq;
        conn.last_ack = packet.ack;

        connections[key] = conn;
        it = connections.find(key);
    }

    Connection& conn = it->second;

    // Update state machine
    updateState(conn, packet);

    // Update sequence tracking
    conn.last_seq = packet.seq;
    conn.last_ack = packet.ack;

    // Se chiusa → cleanup
    if (conn.state == ConnectionState::CLOSED) {
        handleClosedConnection(key);
    }
}

void ConnectionTracker::updateState(Connection& conn, const Packet& packet) {
    // RST ha priorità assoluta
    if (packet.rst) {
        conn.state = ConnectionState::CLOSED;
        return;
    }

    switch (conn.state) {

        case ConnectionState::CLOSED:
            if (packet.syn && !packet.ack) {
                conn.state = ConnectionState::SYN_RECEIVED;
            }
            break;

        case ConnectionState::SYN_RECEIVED:
            if (packet.ack) {
                conn.state = ConnectionState::ESTABLISHED;
            }
            break;

        case ConnectionState::ESTABLISHED:
            if (packet.fin) {
                conn.state = ConnectionState::FIN_WAIT;
            }
            break;

        case ConnectionState::FIN_WAIT:
            if (packet.ack) {
                conn.state = ConnectionState::CLOSED;
            }
            break;
    }
}

void ConnectionTracker::handleClosedConnection(const ConnectionKey& key) {
    // Qui potrai:
    // - salvare su SQL
    // - notificare Redis
    // - loggare evento

    connections.erase(key);
}

std::optional<Connection> ConnectionTracker::getConnection(const ConnectionKey& key) const {
    auto it = connections.find(key);
    if (it != connections.end()) {
        return it->second;
    }
    return std::nullopt;
}

size_t ConnectionTracker::getActiveConnectionsCount() const {
    return connections.size();
}