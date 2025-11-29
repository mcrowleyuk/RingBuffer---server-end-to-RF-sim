#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include "CommunicationSystem.h"

#pragma comment(lib, "Ws2_32.lib")
#include "Ringbuffer.h"

    
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
    // Step 1: Write outgoing data to the send_buffer
    send_buffer.write(data, len);

    // Step 2: Prepare a temporary array to hold send_buffered data
    uint8_t tempsend_buffer[1024];

    // Step 3: Read from the send_buffer into tempsend_buffer
    size_t bytesToSend = send_buffer.read(tempsend_buffer, sizeof(tempsend_buffer));

    //std::cout << "bytes ready to send = " << bytesToSend << "\n";

    // Step 4: Send the send_buffer data over UDP
    if (bytesToSend > 0) 
    {
        int sentBytes = sendto(socket_fd,
            reinterpret_cast<const char*>(tempsend_buffer),
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

    uint8_t recv_buffer[1024]; // raw data from socket message receipt

    uint8_t temp_read_buffer[1024]; // buffer for temp read back from ring buffer

    int receivedBytes = recvfrom(socket_fd, reinterpret_cast<char*>(recv_buffer), sizeof(recv_buffer), 0, nullptr, nullptr);
    std::cout << "number of received bytes = " << receivedBytes << "\n";
    
    
    //now write this to the receive ring buffer (a vector of buffers)
    // this will decouple receiving of message from its display to the user - preventing overload / loss etc.
    receive_buffer.write(recv_buffer, receivedBytes);

    //now read back from the ring buffer
    
    // Step 3: Read from the send_buffer into tempsend_buffer
    size_t bytesRead = receive_buffer.read(temp_read_buffer, sizeof(temp_read_buffer));

    //??? Are we display just the last message or all unread messages here???????
    //****************************************************************************
    
    if (receivedBytes > 0) 
    {
        // display back from tail of ring buffer - number of bytes
        std::cout << "Received: " << std::string(reinterpret_cast<char*>(temp_read_buffer), bytesRead) << std::endl;
    }
}


void CommunicationSystem::listen()
{
    std::cout << "listening in background\n\n\n....";
    // Receive data
    while (true)
    {
        read(); // Keep checking for incoming messages
        
        Sleep(10); // Optional: small delay to prevent CPU overuse
    }
}