#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")
#include "RingBuffer.h"


// Communication System using UDP
class CommunicationSystem {
public:
    CommunicationSystem(const std::string& ip, uint16_t port) : buffer(1024), ip(ip), port(port) {
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup failed" << std::endl;
            exit(EXIT_FAILURE);
        }
        setupSocket();
    }

    ~CommunicationSystem() {
        closesocket(socket_fd);
        WSACleanup();
    }

    // Initialize UDP socket
    void setupSocket() {
        socket_fd = socket(
            AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (socket_fd == INVALID_SOCKET) {
            std::cerr << "Failed to create socket" << std::endl;
            exit(EXIT_FAILURE);
        }

        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);

        if (InetPtonA(AF_INET, ip.c_str(), &serverAddr.sin_addr) != 1) {
            std::cerr << "Invalid IP address format" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    // Send data using UDP
    /*void sendData(const uint8_t* data, size_t len) {
        size_t bytesWritten = buffer.write(data, len);
        if (bytesWritten > 0) {
            int sentBytes = sendto(socket_fd, reinterpret_cast<const char*>(data), static_cast<int>(bytesWritten), 0,
                reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));
            if (sentBytes == SOCKET_ERROR) {
                std::cerr << "Failed to send data" << std::endl;
            }
        }
    }*/

    void sendData(const uint8_t* data, size_t len) {
        // Step 1: Write incoming data to the buffer
        buffer.write(data, len);

        // Step 2: Prepare a temporary array to hold buffered data
        uint8_t tempBuffer[1024];

        // Step 3: Read from the buffer into tempBuffer
        size_t bytesToSend = buffer.read(tempBuffer, sizeof(tempBuffer));

        // Step 4: Send the buffered data over UDP
        if (bytesToSend > 0) {
            int sentBytes = sendto(socket_fd,
                reinterpret_cast<const char*>(tempBuffer),
                static_cast<int>(bytesToSend),
                0,
                reinterpret_cast<sockaddr*>(&serverAddr),
                sizeof(serverAddr));
            if (sentBytes == SOCKET_ERROR) {
                std::cerr << "Failed to send data" << std::endl;
            }
        }
    }


    // Receive data from UDP socket
    void read() {
        uint8_t recvBuffer[1024];
        int receivedBytes = recvfrom(socket_fd, reinterpret_cast<char*>(recvBuffer), sizeof(recvBuffer), 0, nullptr, nullptr);
        if (receivedBytes > 0) {
            std::cout << "Received: " << std::string(reinterpret_cast<char*>(recvBuffer), receivedBytes) << std::endl;
        
            // MAYBE THIS
            buffer.write(recvBuffer, receivedBytes); // Store in buffer
            std::cout << "Received and buffered: " << std::string(reinterpret_cast<char*>(recvBuffer), receivedBytes) << std::endl;
            //
        
        }
    }

private:
    SOCKET socket_fd;
    sockaddr_in serverAddr;
    std::string ip;
    uint16_t port;
    RingBuffer buffer;
    WSADATA wsaData;
};