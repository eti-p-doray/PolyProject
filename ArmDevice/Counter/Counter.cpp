/*
 * Timer.cpp
 *
 * Created: 2014-09-05 14:19:28
 *  Author: Etienne
 */ 

#include "Counter.h"

Counter::Counter(UniquePtr<CounterController>&& counterController, TC_CLKSEL_enum source, uint16_t periode, TC_WGMODE_enum mode)
{
    counterController_ = static_cast<UniquePtr<CounterController>&&>(counterController);
	
	counterController_->module()->CTRLA = 0;
	counterController_->module()->CTRLB = 0;
	counterController_->module()->CTRLC = 0;
	counterController_->module()->CTRLD = 0;
	counterController_->module()->CTRLE = 0;
	counterController_->module()->INTCTRLA = 0;
	counterController_->module()->INTCTRLB = 0;
	
	configure(source, periode, mode);
	tc = counterController_->module();
}

Counter::~Counter() {
    counterController_->module()->CTRLA = 0;
    counterController_->module()->CTRLB = 0;
    counterController_->module()->CTRLC = 0;
    counterController_->module()->CTRLD = 0;
    counterController_->module()->CTRLE = 0;
    counterController_->module()->INTCTRLA = 0;
    counterController_->module()->INTCTRLB = 0;
}

void Counter::configure(TC_CLKSEL_enum source, uint16_t periode, TC_WGMODE_enum mode)
{
	counterController_->module()->CTRLA = source;
	counterController_->module()->CTRLB = mode;
	counterController_->module()->PER = periode;
}

void Counter::configure(TC_CLKSEL_enum source)
{
    counterController_->module()->CTRLA = source;
}

Counter::Channel* Counter::createChannel(Channel::Index channel)
{
	if (channels_[static_cast<uint8_t>(channel)] == nullptr) {
        if ((counterController_->getType() != 0 && counterController_->getType() != 4) && static_cast<uint8_t>(channel) > static_cast<uint8_t>(Channel::Index::B)) {
            return nullptr;
        }
        channels_[static_cast<uint8_t>(channel)] = new Counter::Channel(counterController_.getPtr(), channel);
		return channels_[static_cast<uint8_t>(channel)].getPtr();
	}
	return nullptr;
}

Counter::Channel* Counter::createChannel(PinLocation location, Channel::Index channel)
{
    if (channels_[static_cast<uint8_t>(channel)] == nullptr) {
        if ((counterController_->getType() != 0 && counterController_->getType() != 4) && static_cast<uint8_t>(channel) > static_cast<uint8_t>(Channel::Index::B)) {
            return nullptr;
        }
        channels_[static_cast<uint8_t>(channel)] = new Counter::Channel(counterController_.getPtr(), location, channel);
        return channels_[static_cast<uint8_t>(channel)].getPtr();
    }
    return nullptr;
}

uint16_t Counter::read() const
{
	return counterController_->module()->CNT;
}

void Counter::write(uint16_t value)
{
	counterController_->module()->CNT = value;
}

uint16_t Counter::getPeriode() const
{
    return counterController_->module()->PER;
}

void Counter::listenOverflow(CounterInterrupt::OverflowListenner* listenner, TC_OVFINTLVL_enum interruptLevel)
{
	counterController_->setOverflowListenner(listenner);
	counterController_->module()->INTCTRLA |= interruptLevel;
}