#include <iostream>

#include "CommunicationSystem.h"
#include "Ringbuffer.h"
#include <thread>   

// Sender


int main() {
    // Create a communication system to send/receive data


    std::string ip;

    bool bExit = false;

    std::cout << "Enter IP address to send to: ";

    std::cin >> ip;


    CommunicationSystem comm(ip, 8080); // check IP is correct 
   
    std::thread t(&CommunicationSystem::listen, &comm);


    while (!bExit)
    {
        std::string str;
        std::cout << "Enter a message to send or Q to quit: ";

        std::cin >> str;

        if (str == "Q")
        {
            exit(0);
        }

        if (!bExit)
        {
            std::cout << "sending...." << str << "\n";


            const uint8_t* message = reinterpret_cast<const uint8_t*>(str.c_str());
            comm.sendData(message, str.size());
        }

    }
    

    t.join();
    exit(0);
    return 0;
}
