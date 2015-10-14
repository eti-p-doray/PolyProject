/*
 * CPPlib.cpp
 *
 * Created: 2014-07-30 10:51:59
 *  Author: Etienne
 */ 

#include "stdlib.h"

extern "C" void __cxa_pure_virtual(void);
void __cxa_pure_virtual() {}

void * operator new(size_t size)
{
	return malloc(size);
}

void operator delete(void * ptr)
{
	if (ptr != nullptr) {
		free(ptr);
	}
}

void * operator new[](size_t size)
{
	return malloc(size);
}

void operator delete[](void * ptr)
{
	if (ptr != nullptr) {
		free(ptr);
	}
}

__extension__ typedef int __guard __attribute__((mode (__DI__)));
extern "C" int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);};
extern "C" void __cxa_guard_release (__guard *g) {*(char *)g = 1;};
extern "C" void __cxa_guard_abort (__guard *) {};