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
    RingBuffer(size_t size) : buffer(size), head(0), tail(0), capacity(size) {}

    size_t write(const uint8_t* data, size_t len) 
    {
        size_t bytesWritten = 0;
        for (size_t i = 0; i < len && ((tail + 1) % capacity != head); ++i) 
        {
            //Add the requested data to the buffer providing not filled up ring buffer
            buffer[tail] = data[i];
            tail = (tail + 1) % capacity;
            ++bytesWritten;
        }
        return bytesWritten;
    }

    size_t read(uint8_t* outData, size_t maxLen) 
    {
        size_t bytesRead = 0;
        while (head != tail && bytesRead < maxLen) 
        {
            //read requested number of bytes, place in outData, stop if head == tail
            outData[bytesRead] = buffer[head];
            head = (head + 1) % capacity;
            ++bytesRead;
        }
        return bytesRead;
    }


private:
    std::vector<uint8_t> buffer;
    size_t head, tail, capacity;
};

