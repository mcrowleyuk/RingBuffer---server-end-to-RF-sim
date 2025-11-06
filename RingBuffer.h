#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")

// Dummy RingBuffer class for demonstration
class RingBuffer {
public:
    RingBuffer(size_t size) : buffer(size), head(0), tail(0), capacity(size) {};


    RingBuffer(RingBuffer& rRingBuffer) = delete; // force move semantics instead - see below

    RingBuffer(RingBuffer&& rvRingBuffer) = delete; // to be implemented in due course

    RingBuffer& operator=(RingBuffer& rRingBuffer) = delete; // force move semantics instead - see below

    RingBuffer& operator=(RingBuffer&& rvRingBuffer); // this operator implemented


    // Main buffer read and write methods.

    const size_t write(const uint8_t* data, size_t len);

    const size_t read(uint8_t* outData, size_t maxLen);


private:
    std::vector<uint8_t> buffer;
    size_t head, tail, capacity;
};

