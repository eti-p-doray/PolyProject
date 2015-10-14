/*
 * Timer.h
 *
 * Created: 2014-09-09 10:16:53
 *  Author: Etienne
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include "../Controller/ModuleController.h"
#include "../Counter/Counter.h"

class Timer {
public:
    Timer(UniquePtr<CounterController>&& counterController, uint16_t periode);
	~Timer();
	
	bool isReady();
	void clear();
	
private:
	Counter counter_;
};


#endif /* TIMER_H_ */