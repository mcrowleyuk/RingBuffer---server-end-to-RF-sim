#include <iostream>
#include <atomic>
#include <cstring> // For memset

// Ring Buffer class for handling communication data
class RingBuffer {
public:
    RingBuffer(size_t size) : bufferSize(size), head(0), tail(0), buffer(new uint8_t[size]) {}

    ~RingBuffer() {
        delete[] buffer;
    }

    // Write data into the buffer (returns the number of bytes written)
    size_t write(const uint8_t* data, size_t len) {
        size_t bytesWritten = 0;
        while (bytesWritten < len) {
            if ((head + 1) % bufferSize != tail) {  // Check if buffer is not full
                buffer[head] = data[bytesWritten];
                head = (head + 1) % bufferSize;
                bytesWritten++;
            } else {
                break; // Buffer is full, stop writing
            }
        }
        return bytesWritten;
    }

    // Read data from the buffer (returns the number of bytes read)
    size_t read(uint8_t* data, size_t len) {
        size_t bytesRead = 0;
        while (bytesRead < len) {
            if (head != tail) {  // Check if buffer is not empty
                data[bytesRead] = buffer[tail];
                tail = (tail + 1) % bufferSize;
                bytesRead++;
            } else {
                break; // Buffer is empty, stop reading
            }
        }
        return bytesRead;
    }

private:
    size_t bufferSize;
    size_t head, tail;
    uint8_t* buffer;
};