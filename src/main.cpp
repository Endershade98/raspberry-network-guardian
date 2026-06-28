// src/main.cpp

#include <network_guardian/infrastructure/RawSocketSniffer.hpp>

#include <atomic>
#include <csignal>
#include <iostream>
#include <string>


namespace
{

std::atomic<bool> running{true};


void signalHandler(int signal)
{
    std::cout
        << "\n[INFO] Shutdown signal received: "
        << signal
        << "\n";

    running = false;
}


void registerSignals()
{
    std::signal(
        SIGINT,
        signalHandler
    );

    std::signal(
        SIGTERM,
        signalHandler
    );
}


std::string defaultInterface()
{
    return "eth0";
}


}



int main(
    int argc,
    char* argv[]
)
{

    registerSignals();


    std::string interface =
        defaultInterface();



    if (argc > 1)
    {
        interface = argv[1];
    }



    std::cout
        << "[INFO] Raspberry Network Guardian starting\n";


    std::cout
        << "[INFO] Interface: "
        << interface
        << "\n";



    RawSocketSniffer sniffer(interface);



    sniffer.setCallback(
        [](const uint8_t*, std::size_t length)
        {

            std::cout
                << "[PACKET] captured "
                << length
                << " bytes\n";

        }
    );



    if (!sniffer.start())
    {

        std::cerr
            << "[ERROR] Failed to start packet sniffer\n";

        return EXIT_FAILURE;
    }



    std::cout
        << "[INFO] Capture started\n";


    while (running)
    {
        // Future:
        // CLI commands
        // metrics
        // persistence loop
        // state monitoring

        std::this_thread::sleep_for(
            std::chrono::milliseconds(200)
        );
    }



    std::cout
        << "[INFO] Stopping capture...\n";


    sniffer.stop();



    std::cout
        << "[INFO] Shutdown completed\n";


    return EXIT_SUCCESS;
}