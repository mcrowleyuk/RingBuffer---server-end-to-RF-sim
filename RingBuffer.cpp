#include <iostream>

#include "CommunicationSystem.h"
#include "Ringbuffer.h"
#include <thread>   

int main() {
    // Create a communication system to send/receive data
    CommunicationSystem comm("127.0.0.1", 8080);

    std::thread t(&CommunicationSystem::listen, &comm);


    // Sample data to send
    const uint8_t message[] = "Hello, Real-Time Communication!";
    comm.sendData(message, sizeof(message));


    t.join();

    return 0;
}
