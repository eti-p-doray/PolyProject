/*
 * Node.h
 *
 * Created: 2014-08-09 10:57:14
 *  Author: Etienne
 */ 


#ifndef NODE_H_
#define NODE_H_

#include <stdint.h>

template <class T>
class Node {
public:
	Node(T data);
	const T& getData() const;
    T& getData();
	Node* getNext();
	void setNext(Node* next);
private:
	T data_;
	Node* next_;
};

template <class T>
Node<T>::Node(const T data): data_(data)
{
	setNext(nullptr);
}

template <class T>
const T& Node<T>::getData() const
{
	return data_;
}

template <class T>
T& Node<T>::getData()
{
    return data_;
}

template <class T>
void Node<T>::setNext(Node* next)
{
	next_ = next;
}

template <class T>
Node<T>* Node<T>::getNext()
{
	return next_;
}


#endif /* NODE_H_ */