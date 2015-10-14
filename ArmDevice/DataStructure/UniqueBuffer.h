//
//  UniqueBuffer.h
//  ArmDevice
//
//  Created by Etienne Pierre-Doray on 2015-02-19.
//
//

#ifndef UNIQUE_BUFFER_H_
#define UNIQUE_BUFFER_H_

#include "stdint.h"
#include "stdlib.h"

template <class T>
class UniqueBuffer {
public:
    UniqueBuffer(uint16_t lenght);
    UniqueBuffer(const UniqueBuffer<T>& b);
    UniqueBuffer<T>& operator=(const UniqueBuffer<T>& b);
    ~UniqueBuffer();
    
    void write(T data, uint16_t i);
    const T* read() const {return data_;}
    T* read() {return data_;}
    T read(uint16_t i) const;
    uint16_t lenght() const {return lenght_;}
    
private:
    T* data_;
    uint16_t lenght_;
};

template <class T>
UniqueBuffer<T>::UniqueBuffer(uint16_t lenght) : lenght_(lenght)
{
    data_ = new T[lenght_];
}

template <class T>
UniqueBuffer<T>::UniqueBuffer(const UniqueBuffer<T>& b)
{
    lenght_ = b.lenght_;
	data_ = new T[lenght_];
    for (uint16_t i = 0; i < lenght_; i++) {
        data_ = b.data_;
    }
}

template <class T>
UniqueBuffer<T>& UniqueBuffer<T>::operator=(const UniqueBuffer<T>& b)
{
	if (this != &b) {
		delete [] data_;
		lenght_ = b.lenght_;
		data_ = new T[lenght_];
		for (uint16_t i = 0; i < lenght_; i++) {
			data_ = b.data_;
		}
		return *this;
	}
}

template <class T>
UniqueBuffer<T>::~UniqueBuffer()
{
    delete [] data_;
}

template <class T>
void UniqueBuffer<T>::write(T data, uint16_t i) {
    data_[i] = data;
}

template <class T>
T UniqueBuffer<T>::read(uint16_t i) const
{
    return data_[i];
}

#endif /* defined(__ArmDevice__UniqueBuffer__) */
