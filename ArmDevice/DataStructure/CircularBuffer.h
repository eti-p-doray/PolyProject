//
//  CircularBuffer.h
//  ArmDevice
//
//  Created by Etienne Pierre-Doray on 2015-02-11.
//
//

#ifndef ArmDevice_CircularBuffer_h
#define ArmDevice_CircularBuffer_h

#include "stdint.h"
#include "stdlib.h"

template <class T>
class CircularBuffer {
public:
    CircularBuffer(uint16_t lenght);
    ~CircularBuffer();
    
    void write(T data) volatile;
    T read() volatile;
    uint16_t lenght() const volatile {return lenght_;}
    
    bool isEmpty() const volatile {return writeIndex_ == readIndex_;}
    bool isFull() const volatile {return (writeIndex_+1)%lenght_ == readIndex_;}
    void reset() volatile {writeIndex_ = 0; readIndex_ = 0;}
    
private:
    T* data_;
    uint16_t lenght_;
    uint16_t writeIndex_;
    uint16_t readIndex_;
};

template <class T>
CircularBuffer<T>::CircularBuffer(uint16_t lenght)
{
    data_ = new T[lenght];
    lenght_ = lenght;
	reset();
}

template <class T>
CircularBuffer<T>::~CircularBuffer()
{
    delete [] data_;
}

template <class T>
void CircularBuffer<T>::write(T data) volatile
{
    data_[writeIndex_] = data;
    writeIndex_ = (writeIndex_ + 1) % lenght_;
}

template <class T>
T CircularBuffer<T>::read() volatile
{
    uint16_t i = readIndex_;
    readIndex_ = (readIndex_ + 1) % lenght_;
    return data_[i];
}

#endif
