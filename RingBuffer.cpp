#include "RingBuffer.h"

const size_t RingBuffer::write(const uint8_t* data, size_t len)
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


const size_t RingBuffer::read(uint8_t* outData, size_t maxLen)
{
    size_t bytesRead = 0;
    while (head != tail && bytesRead < maxLen)
    {
        //read requested number of bytes, place in outData, stop if head == tail
        outData[bytesRead] = buffer[head];
        //std::cout << "buffer count is " << buffer.size() << "\n";
        head = (head + 1) % capacity;
        ++bytesRead;
    }
    return bytesRead;
}

//move assignment operator
RingBuffer& RingBuffer::operator=(RingBuffer&& rvRingBuffer)
{
    // Cater for scenario where Buffer is reassigned but r value ref no longer required.

    if (&rvRingBuffer != this)
    {

        buffer = std::move(rvRingBuffer.buffer);  // cast buffer to an RV so ideally std::vector will use move assignment

        head = rvRingBuffer.head;
        tail = rvRingBuffer.tail;
        capacity = rvRingBuffer.capacity;
    }
    return *this;
}

//move constuctor
RingBuffer::RingBuffer(RingBuffer&& rvRingBuffer) //Initialise new Lvalue by stealing resources from RV
{
    buffer = std::move(rvRingBuffer.buffer);  // cast buffer to an RV so ideally std::vector will use move assignment

    head = rvRingBuffer.head;
    tail = rvRingBuffer.tail;
    capacity = rvRingBuffer.capacity;

}

//copy constructor
RingBuffer::RingBuffer(RingBuffer& rRingBuffer)
{
    buffer = rRingBuffer.buffer;  // std::vector will perform deep copy, care needed here but okay in this context.

    head = rRingBuffer.head;
    tail = rRingBuffer.tail;
    capacity = rRingBuffer.capacity;
}

