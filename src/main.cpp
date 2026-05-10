// src/main.cpp

#include "infrastructure/RawSocketSniffer.h"
#include <iostream>
#include <csignal>
#include <unistd.h>
using namespace std;


int main() {
    RawSocketSniffer sniffer("eth0");

    sniffer.setCallback([](const uint8_t*, size_t len) {
        std::cout << "Packet captured: " << len << " bytes\n";
    });

    if (!sniffer.start()) {
        std::cerr << "Failed\n";
        return 1;
    }

    std::cout << "Running...\n";

    pause(); // oppure CLI controller

    sniffer.stop();
}