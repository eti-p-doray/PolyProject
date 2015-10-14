/*
 * InterruptController.h
 *
 * Created: 2014-07-29 10:07:23
 *  Author: Etienne
 */ 


#ifndef INTERRUPTCONTROLLER_H_
#define INTERRUPTCONTROLLER_H_

#include "avr/io.h"

//enum class InterruptLevel : uint8_t {high};

class InterruptController {
public:
	InterruptController();
	~InterruptController();
	
private:
	static uint8_t nUsers;
};

#endif /* INTERRUPTCONTROLLER_H_ */