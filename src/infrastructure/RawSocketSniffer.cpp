// src/infrastructure/RawSocketSniffer.cpp

#include <network_guardian/infrastructure/RawSocketSniffer.hpp>

#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <cstring>
#include <cerrno>
#include <iostream>
#include <netinet/in.h>


RawSocketSniffer::RawSocketSniffer(
    const std::string& iface
)
    :
    sockfd(-1),
    interface(iface),
    running(false)
{
}


RawSocketSniffer::~RawSocketSniffer()
{
    stop();
}



bool RawSocketSniffer::start()
{
    if(running)
        return true;



    sockfd =
        socket(
            AF_PACKET,
            SOCK_RAW,
            htons(ETH_P_ALL)
        );


    if(sockfd < 0)
    {
        perror("socket");
        return false;
    }



    timeval timeout{};

    timeout.tv_sec = 1;
    timeout.tv_usec = 0;



    setsockopt(
        sockfd,
        SOL_SOCKET,
        SO_RCVTIMEO,
        &timeout,
        sizeof(timeout)
    );



    ifreq ifr{};


    strncpy(
        ifr.ifr_name,
        interface.c_str(),
        IFNAMSIZ-1
    );


    if(ioctl(
        sockfd,
        SIOCGIFINDEX,
        &ifr
    ) < 0)
    {
        perror("ioctl");
        close(sockfd);
        sockfd=-1;
        return false;
    }



    sockaddr_ll addr{};

    addr.sll_family =
        AF_PACKET;

    addr.sll_ifindex =
        ifr.ifr_ifindex;

    addr.sll_protocol =
        htons(ETH_P_ALL);



    if(bind(
        sockfd,
        reinterpret_cast<sockaddr*>(&addr),
        sizeof(addr)
    ) < 0)
    {
        perror("bind");

        close(sockfd);
        sockfd=-1;

        return false;
    }



    running=true;


    worker =
        std::thread(
            &RawSocketSniffer::captureLoop,
            this
        );


    return true;
}



void RawSocketSniffer::stop()
{
    running=false;


    if(worker.joinable())
        worker.join();



    if(sockfd!=-1)
    {
        close(sockfd);
        sockfd=-1;
    }
}




void RawSocketSniffer::captureLoop()
{

    uint8_t buffer[BUFFER_SIZE];



    while(running)
    {

        ssize_t len =
            recvfrom(
                sockfd,
                buffer,
                sizeof(buffer),
                0,
                nullptr,
                nullptr
            );



        if(!running)
            break;



        if(len < 0)
        {
            if(
                errno==EAGAIN ||
                errno==EWOULDBLOCK ||
                errno==EINTR
            )
                continue;


            break;
        }



        if(len==0)
            continue;



        if(callback)
        {


            /*
             * Loopback Linux:
             *
             * lo può arrivare come:
             *
             *  16 byte cooked header
             *  oppure
             *  ethernet fake header
             *
             */


            if(interface=="lo")
            {

                /*
                 * Linux loopback cooked header
                 * protocol field:
                 *
                 * offset 14-15
                 */

                uint16_t proto =
                    ntohs(
                        *reinterpret_cast<uint16_t*>(
                            buffer + 14
                        )
                    );


                if(proto == ETH_P_IP)
                {

                    callback(
                        buffer + 16,
                        len - 16
                    );

                    continue;
                }
            }



            callback(
                buffer,
                static_cast<size_t>(len)
            );
        }
    }
}



void RawSocketSniffer::setCallback(
    PacketCallback cb
)
{
    callback =
        std::move(cb);
}