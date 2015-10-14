/*
 * Queue.h
 *
 * Created: 2014-07-17 20:29:21
 *  Author: Etienne
 */ 


#ifndef QUEUE_H_
#define QUEUE_H_

#include "Node.h"

#include <stdint.h>

template<class T>
class Queue {
public:
	Queue();
	~Queue();
	
	const T& front() const volatile;
	const T& back() const volatile;
    T& front() volatile;
    T& back() volatile;
	void pop() volatile;
	void push(T data) volatile;
	bool isEmpty() const volatile;
	void reset() volatile;
	
private:
	Node<T>* first_;
	Node<T>* last_;
};

template <class T>
Queue<T>::Queue() {
	first_ = nullptr;
	last_ = nullptr;
}

template <class T>
Queue<T>::~Queue() {
	reset();
}

template <class T>
const T& Queue<T>::front() const volatile
{
	return first_->getData();
}

template <class T>
const T& Queue<T>::back() const volatile
{
	return last_->getData();
}

template <class T>
T& Queue<T>::front() volatile
{
    return first_->getData();
}

template <class T>
T& Queue<T>::back() volatile
{
    return last_->getData();
}

template <class T>
void Queue<T>::pop() volatile
{
	Node<T>* first = first_->getNext();
	delete first_;
	first_ = first;
}

template <class T>
void Queue<T>::push(T data) volatile
{
	Node<T>* last = new Node<T>(data);
	if (first_ != nullptr) {
		last_->setNext(last);
	}
	else {
		first_ = last;
	}
	last_ = last;
}

template <class T>
bool Queue<T>::isEmpty() const volatile
{
	return (first_==nullptr);
}

template <class T>
void Queue<T>::reset() volatile
{
	while (!isEmpty()) {
		pop();
	}
}


#endif /* QUEUE_H_ */