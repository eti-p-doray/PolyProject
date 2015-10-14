/*
 * Channel.cpp
 *
 * Created: 2014-09-07 10:57:30
 *  Author: Etienne
 */ 

#include "Counter.h"

void Counter::Channel::write(uint16_t value)
{
	volatile uint16_t* compareRegister = &(counterController_->module()->CCABUF)+static_cast<uint8_t>(channelIndex_);
	*compareRegister = value;
}

uint16_t Counter::Channel::read() const
{
    return *(&(counterController_->module()->CCA)+static_cast<uint8_t>(channelIndex_));
}

void Counter::Channel::listen(CounterInterrupt::ChannelListenner& listenner, TC_CCAINTLVL_enum interruptLevel)
{
    counterController_->setChannelListenner(&listenner, static_cast<uint8_t>(channelIndex_));
	counterController_->module()->INTCTRLA |= interruptLevel << static_cast<uint8_t>(channelIndex_) * 2;
}

void Counter::Channel::enablePin(PinLocation location)
{
    counterController_->module()->CTRLB |= TC0_CCAEN_bm << static_cast<uint8_t>(channelIndex_);
	//counterController_->module()->CTRLC |= (TC0_POLA_bm << static_cast<uint8_t>(channelIndex_));
    if (counterController_->getType() == 0 || counterController_->getType() == 4) {
        counterController_->getPort()->DIR |= ( 1<< ( static_cast<uint8_t>(channelIndex_)+static_cast<bool>(location)*4) );
		counterController_->getPort()->REMAP |= (static_cast<bool>(location) << static_cast<uint8_t>(channelIndex_) );
    }
    else {
        counterController_->getPort()->DIR |= ( 1<< ( static_cast<uint8_t>(channelIndex_)+4) );
    }
}

void Counter::Channel::disablePin()
{
    counterController_->module()->CTRLB &= ~( TC0_CCAEN_bm << static_cast<uint8_t>(channelIndex_) );
	//counterController_->module()->CTRLC &= ~(TC4_POLA_bm << static_cast<uint8_t>(channelIndex_));
    if (counterController_->getType() == 0 || counterController_->getType() == 4) {
        counterController_->getPort()->DIR &= ~( 1<< ( static_cast<uint8_t>(channelIndex_)+((counterController_->getPort()->REMAP&(1<<static_cast<uint8_t>(channelIndex_)))!=0)*4) );
        counterController_->getPort()->REMAP &= ~(1 << static_cast<uint8_t>(channelIndex_) );
    }
    else {
        counterController_->getPort()->DIR &= ~( 1<< ( static_cast<uint8_t>(channelIndex_)+4) );
    }
}

void Counter::Channel::map(PinLocation location)
{
    if (counterController_->getType() != 0 ) {
        return;
    }
	bool pinEnabled = (counterController_->module()->CTRLE & (TC0_CCAEN_bm << static_cast<uint8_t>(channelIndex_))) != 0;
	if (pinEnabled) {
		disablePin();
		enablePin(location);
	}
}

Counter::Channel::Channel(CounterController* counterController, Index channelIndex)
{
	counterController_ = counterController;
    channelIndex_ = channelIndex;
}

Counter::Channel::Channel(CounterController* counterController, PinLocation location, Index channelIndex)
{
	tc = counterController->module();
	port = counterController->getPort();
	
    counterController_ = counterController;
    channelIndex_ = channelIndex;
    counterController_->module()->CTRLB |= TC0_CCAEN_bm << static_cast<uint8_t>(channelIndex_);
    enablePin(location);
}

Counter::Channel::~Channel()
{
    disablePin();
	counterController_->module()->CTRLB &= ~( TC0_CCAEN_bm << 2*static_cast<uint8_t>(channelIndex_) );
}