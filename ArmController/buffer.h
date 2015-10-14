/*
 * Buffer.h
 *
 * Created: 2014-07-17 20:17:12
 *  Author: Etienne
 */


#ifndef BUFFER_H_
#define BUFFER_H_

#include "stdint.h"
#include "stdlib.h"

template <class T>
class Buffer {
public:
    Buffer();
    Buffer(T* data, uint16_t lenght);
    Buffer(const T* data, uint16_t lenght);

    void write(T data, uint16_t i);
    const T* read() const {return data_;}
    T read(uint16_t i) const;
    uint16_t lenght() const {return lenght_;}

private:
    T* data_;
    uint16_t lenght_;
};

template <class T>
Buffer<T>::Buffer(T* data, uint16_t lenght) : data_(data), lenght_(lenght)
{
}

template <class T>
Buffer<T>::Buffer(const T* data, uint16_t lenght) : data_(data), lenght_(lenght)
{
}

template <class T>
Buffer<T>::Buffer()
{
    data_ = nullptr;
    lenght_ = 0;
}

template <class T>
void Buffer<T>::write(T data, uint16_t i) {
    data_[i] = data;
}

template <class T>
T Buffer<T>::read(uint16_t i) const
{
    return data_[i];
}


#endif /* BUFFER_H_ */
