#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include "CommunicationSystem.h"

#pragma comment(lib, "Ws2_32.lib")
#include "RingBuffer.h"

    
// Initialize UDP socket
void CommunicationSystem::setupSocket()
{
    socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_fd == INVALID_SOCKET)
    {
        std::cerr << "Failed to create socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Bind to local address for receiving
    sockaddr_in localAddr;
    memset(&localAddr, 0, sizeof(localAddr));
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(port); // Same port you're using
    localAddr.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces

    if (bind(socket_fd, reinterpret_cast<sockaddr*>(&localAddr), sizeof(localAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Setup remote address for sending
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if (InetPtonA(AF_INET, ip.c_str(), &serverAddr.sin_addr) != 1)
    {
        std::cerr << "Invalid IP address format" << std::endl;
        exit(EXIT_FAILURE);
    }
}




void CommunicationSystem::sendData(const uint8_t* data, size_t len) {
    // Step 1: Write incoming data to the buffer
    buffer.write(data, len);

    // Step 2: Prepare a temporary array to hold buffered data
    uint8_t tempBuffer[1024];

    // Step 3: Read from the buffer into tempBuffer
    size_t bytesToSend = buffer.read(tempBuffer, sizeof(tempBuffer));


    std::cout << "bytes to send = " << bytesToSend << "\n";

    // Step 4: Send the buffered data over UDP
    if (bytesToSend > 0) 
    {
        int sentBytes = sendto(socket_fd,
            reinterpret_cast<const char*>(tempBuffer),
            static_cast<int>(bytesToSend),
            0,
            reinterpret_cast<sockaddr*>(&serverAddr),
            sizeof(serverAddr));
        if (sentBytes == SOCKET_ERROR) 
        {
            std::cerr << "Failed to send data" << std::endl;
        }
    }
}


// Receive data from UDP socket
void CommunicationSystem::read() 
{
    // TEMP
    const uint8_t message[] = "Hello, Real-Time Communication!";
    sendData(message, sizeof(message));
    // END TEMP

    uint8_t recvBuffer[1024];
    int receivedBytes = recvfrom(socket_fd, reinterpret_cast<char*>(recvBuffer), sizeof(recvBuffer), 0, nullptr, nullptr);
    std::cout << "number of received bytes = " << receivedBytes << "\n";
    if (receivedBytes > 0) 
    {
        std::cout << "Received: " << std::string(reinterpret_cast<char*>(recvBuffer), receivedBytes) << std::endl;

        buffer.write(recvBuffer, receivedBytes); // Store in buffer
        std::cout << "Received and buffered: " << std::string(reinterpret_cast<char*>(recvBuffer), receivedBytes) << std::endl;
     
    }
}


void CommunicationSystem::listen()
{
    std::cout << "listening....";
    // Receive data
    while (true)
    {
        read(); // Keep checking for incoming messages
        
        Sleep(10); // Optional: small delay to prevent CPU overuse
    }
}