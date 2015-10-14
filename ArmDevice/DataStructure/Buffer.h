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
#include "UniqueBuffer.h"

template <class T>
class Buffer {
public:
    Buffer();
    Buffer(const Buffer<T>& b) = default;
	Buffer(const volatile Buffer<T>& b);
    Buffer(T* data, uint16_t lenght);
    Buffer(const T* data, uint16_t lenght);
    Buffer(UniqueBuffer<T>& b);
    Buffer<T>& operator=(const Buffer<T>& b) = default;
	
	void write(T data, uint16_t i) volatile;
    const T* read() const volatile {return data_;}
	T read(uint16_t i) const volatile;
	uint16_t lenght() const volatile {return lenght_;}
	
private:
	T* data_;
	uint16_t lenght_;
};

template <class T>
Buffer<T>::Buffer(T* data, uint16_t lenght)
{
	data_ = data;
	lenght_ = lenght;
}

template <class T>
Buffer<T>::Buffer(const T* data, uint16_t lenght) : data_(data), lenght_(lenght)
{
}

template <class T>
Buffer<T>::Buffer(const volatile Buffer<T>& b)
{
	data_ = b.data_;
	lenght_ = b.lenght();
}

template <class T>
Buffer<T>::Buffer(UniqueBuffer<T>& b)
{
    data_ = b.data_;
    lenght_ = b.lenght();
}

template <class T>
Buffer<T>::Buffer()
{
    data_ = nullptr;
    lenght_ = 0;
}

template <class T>
void Buffer<T>::write(T data, uint16_t i) volatile {
	data_[i] = data;
}

template <class T>
T Buffer<T>::read(uint16_t i) const volatile
{
	return data_[i];
}


#endif /* BUFFER_H_ */