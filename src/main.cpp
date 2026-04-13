// src/main.cpp
#include "infrastructure/RawSocketSniffer.h"
#include <iostream>
#include <csignal>
#include <unistd.h>

using namespace std;
using namespace infrastructure;

void signalHandler(int signum) {
    cout << "Interrupt signal (" << signum << ") received.\n";
    exit(signum);
}

int main() {

    RawSocketSniffer sniffer("eth0");

    sniffer.setCallback([](const uint8_t* data, size_t len) {
        cout << "Packet captured: " << len << " bytes\n";
    });

    if (!sniffer.start()) {
        cerr << "Failed to start sniffer\n";
        return 1;
    }

    cout << "Sniffer running... Ctrl+C to stop\n";

    signal(SIGINT, signalHandler);
    pause();
}