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
    void setupSocket();


    void sendData(const uint8_t* data, size_t len);

    // Receive data from UDP socket
    void read();

    void listen();

private:
    SOCKET socket_fd;
    sockaddr_in serverAddr;
    std::string ip;
    uint16_t port;
    RingBuffer buffer;
    WSADATA wsaData;
};