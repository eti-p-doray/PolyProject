/*Auto generated code*/

#include "ModuleController.h"

USART_t* UsartController::Modules[] = {(USART_t*)&USARTC0, (USART_t*)&USARTC1, (USART_t*)&USARTD0, (USART_t*)&USARTD1, (USART_t*)&USARTE0, (USART_t*)&USARTE1, (USART_t*)&USARTF0, (USART_t*)&USARTF1};

PORT_t* UsartController::Ports[] = {&PORTC, &PORTC, &PORTD, &PORTD, &PORTE, &PORTE, &PORTF, &PORTF};

const uint8_t UsartController::Type[] = {0, 1, 0, 1, 0, 1, 0, 1};

UsartController* UsartController::instances_[] = {};
UniquePtr<UsartController> UsartController::instance(UsartController::Index index)
{
	if (instances_[static_cast<uint8_t>(index)]==nullptr) {
		instances_[static_cast<uint8_t>(index)] = new UsartController(index, &instances_[static_cast<uint8_t>(index)]);
		return UniquePtr<UsartController>(instances_[static_cast<uint8_t>(index)]);
	};
	return UniquePtr<UsartController>();
}

TC0_t* CounterController::Modules[] = {(TC0_t*)&TCC0, (TC0_t*)&TCD0, (TC0_t*)&TCE0, (TC0_t*)&TCF0, (TC0_t*)&TCC1, (TC0_t*)&TCD1, (TC0_t*)&TCE1, (TC0_t*)&TCF1};

PORT_t* CounterController::Ports[] = {&PORTC, &PORTD, &PORTE, &PORTF, &PORTC, &PORTD, &PORTE, &PORTF};

const uint8_t CounterController::Type[] = {0, 0, 0, 0, 1, 1, 1, 1};

CounterController* CounterController::instances_[] = {};
UniquePtr<CounterController> CounterController::instance(CounterController::Index index)
{
	if (instances_[static_cast<uint8_t>(index)]==nullptr) {
		instances_[static_cast<uint8_t>(index)] = new CounterController(index, &instances_[static_cast<uint8_t>(index)]);
		return UniquePtr<CounterController>(instances_[static_cast<uint8_t>(index)]);
	};
	return UniquePtr<CounterController>();
}

TWI_t* TwiController::Modules[] = {(TWI_t*)&TWIC, (TWI_t*)&TWID, (TWI_t*)&TWIE, (TWI_t*)&TWIF};

TwiController* TwiController::instances_[] = {};
UniquePtr<TwiController> TwiController::instance(TwiController::Index index)
{
	if (instances_[static_cast<uint8_t>(index)]==nullptr) {
		instances_[static_cast<uint8_t>(index)] = new TwiController(index, &instances_[static_cast<uint8_t>(index)]);
		return UniquePtr<TwiController>(instances_[static_cast<uint8_t>(index)]);
	};
	return UniquePtr<TwiController>();
}

PORT_t* PortController::Modules[] = {(PORT_t*)&PORTA, (PORT_t*)&PORTB, (PORT_t*)&PORTC, (PORT_t*)&PORTD, (PORT_t*)&PORTE, (PORT_t*)&PORTF, (PORT_t*)&PORTH, (PORT_t*)&PORTJ, (PORT_t*)&PORTK, (PORT_t*)&PORTQ};

PortController* PortController::instances_[] = {};
UniquePtr<PortController> PortController::instance(PortController::Index index)
{
	if (instances_[static_cast<uint8_t>(index)]==nullptr) {
		instances_[static_cast<uint8_t>(index)] = new PortController(index, &instances_[static_cast<uint8_t>(index)]);
		return UniquePtr<PortController>(instances_[static_cast<uint8_t>(index)]);
	};
	return UniquePtr<PortController>();
}

