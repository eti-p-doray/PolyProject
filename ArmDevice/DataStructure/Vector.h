/*
 * Vector.h
 *
 * Created: 2014-07-22 19:42:07
 *  Author: Etienne
 */ 


#ifndef VECTOR_H_
#define VECTOR_H_


template <class T>
class Vector {
public:
	Vector();
	~Vector();
	Vector(const Vector<T> & b);
	Vector<T>& operator=(const Vector<T> & b);
	
	void push(T data);
	void push(Vector<T> data);
	T& operator[](uint8_t index);
	uint8_t lenght() const;
	
private:
	T* array_;
	uint8_t arrayLenght_;
	uint8_t lenght_;
	
	void doublelenght();
};

template <class T>
Vector<T>::Vector() {
	array_ = new T[1];
	arrayLenght_ = 1;
	lenght_ = 0;
}

template <class T>
Vector<T>::~Vector() {
	delete[] array_;
}

template <class T>
Vector<T>::Vector(const Vector<T> & b) {
	*this = Vector(b);
}

template <class T>
Vector<T>& Vector<T>::operator=(const Vector<T> & b) {
	for (uint8_t i=0; i<lenght_; i++) {
		array_[i] = b.array_[i];
	}
	return *this;
}

template <class T>
void Vector<T>::push(T data) {
	if (lenght_>=arrayLenght_) {
		doublelenght();
	}
	array_[lenght_] = data;
	lenght_++;
}

template <class T>
void Vector<T>::push(Vector<T> data) {
	while (lenght_ + data.lenght() >= arrayLenght_) {
		doublelenght();
	}
	for (uint8_t i=0; i<data.lenght(); i++) {
		array_[lenght_+i] = data[i];
	}
	lenght_ += data.lenght();
}

template <class T>
T& Vector<T>::operator[](uint8_t index) {
	while (index >= arrayLenght_) {
		doublelenght();
	}
	if (index >= lenght_) {
		lenght_ = index+1;
	}
	return array_[index];
}

template <class T>
uint8_t Vector<T>::lenght() const {
	return lenght_;
}

template <class T>
void Vector<T>::doublelenght() {
	T* array = new T[2*arrayLenght_];
	for (uint8_t i=0; i<arrayLenght_; i++) {
		array[i] = array_[i];
	}
	delete[] array_;
	array_ = array;
	arrayLenght_ *= 2;
}



#endif /* VECTOR_H_ */