/*
 * Timer.cpp
 *
 * Created: 2014-09-09 10:20:27
 *  Author: Etienne
 */ 


#include "Timer.h"

Timer::Timer(UniquePtr<CounterController>&& counterController, uint16_t periode) :
    counter_(static_cast<UniquePtr<CounterController>&&>(counterController), TC_CLKSEL_OFF_gc, 0, TC_WGMODE_NORMAL_gc)
{
	TC_CLKSEL_enum prescaler = TC_CLKSEL_DIV1_gc;
	uint8_t pres = 0;
	if (periode<=((uint32_t)1<<16)/(F_CPU/1000)) {
		pres = 0;
		prescaler = TC_CLKSEL_DIV1_gc;
	}
	else if (periode<=((uint32_t)1<<(16+1))/(F_CPU/1000)) {
		pres = 1;
		prescaler = TC_CLKSEL_DIV2_gc;
	}
	else if (periode<=((uint32_t)1<<(16+2))/(F_CPU/1000)) {
		pres = 2;
		prescaler = TC_CLKSEL_DIV4_gc;
	}
	else if (periode<=((uint32_t)1<<(16+3))/(F_CPU/1000)) {
		pres = 3;
		prescaler = TC_CLKSEL_DIV8_gc;
	}
	else if (periode<=((uint32_t)1<<(16+6))/(F_CPU/1000)) {
		pres = 6;
		prescaler = TC_CLKSEL_DIV64_gc;
	}
	else if (periode<=((uint32_t)1<<(16+8))/(F_CPU/1000)) {
		pres = 8;
		prescaler = TC_CLKSEL_DIV256_gc;
	}
	else if (periode<=((uint32_t)1<<(16+10))/(F_CPU/1000)) {
		pres = 10;
		prescaler = TC_CLKSEL_DIV1024_gc;
	}
	counter_.configure(prescaler, periode*((int32_t)F_CPU>>pres)/1000, TC_WGMODE_NORMAL_gc);
}

Timer::~Timer() {
	
}

bool Timer::isReady()
{
	return counter_.isOverflow();
}

void Timer::clear()
{
	counter_.clearOverflow();
}
