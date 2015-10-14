/*
 * InterruptController.cpp
 *
 * Created: 2014-07-30 11:30:23
 *  Author: Etienne
 */ 

#include "InterruptController.h"
#include "avr/interrupt.h"

uint8_t InterruptController::nUsers = 0;

InterruptController::InterruptController() {
	nUsers += 1;
	PMIC.CTRL |= PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm;
	sei();
}

InterruptController::~InterruptController() {
	nUsers -= 1;
	if (nUsers == 0) {
		PMIC.CTRL = 0;
		SREG &= ~SREG_I;
	}
}