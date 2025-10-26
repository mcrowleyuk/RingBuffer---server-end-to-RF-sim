#include <iostream>

#include "CommunicationSystem.h"
#include "Ringbuffer.h"

int main() {
    // Create a communication system to send/receive data
    CommunicationSystem comm("127.0.0.1", 8080);

    // Sample data to send
    const uint8_t message[] = "Hello, Real-Time Communication!";
    comm.sendData(message, sizeof(message));

    // Receive data
    comm.read();

    return 0;
}
