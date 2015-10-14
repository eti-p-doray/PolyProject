//
//  Servo.cpp
//  ArmDevice
//
//  Created by Etienne Pierre-Doray on 2015-02-09.
//
//

#include "Servo.h"

void Servo::write(int16_t value)
{
    channel_->write( (int32_t(value * int32_t(scale_)) >> 16) + delta_ );
}

void Servo::map(PinLocation location)
{
    channel_->map(location);
}

void Servo::setRange(uint16_t minPulse, uint16_t maxPulse)
{
    delta_ = float(maxPulse + minPulse) * float(frequency_) * float(channel_->getPeriode()) * 0.5e-6f;
    scale_ = float(maxPulse - minPulse) * float(frequency_) * float(channel_->getPeriode()) * 1.0e-6f;
}

Servo::Servo(Counter::Channel* channel, uint16_t frequency)
{
    channel_ = channel;
    frequency_ = frequency;
}

ServoController::ServoController(UniquePtr<CounterController>&& counterController, uint16_t frequency) :
    counter_(static_cast<UniquePtr<CounterController>&&>(counterController), TC_CLKSEL_OFF_gc, 0, TC_WGMODE_DSTOP_gc)
{
    TC_CLKSEL_enum prescaler = TC_CLKSEL_DIV1_gc;
    uint8_t pres = 0;
    if (frequency >= F_CPU/((uint32_t)1<<(16+1)) ) {
        pres = 1;
        prescaler = TC_CLKSEL_DIV1_gc;
    }
    else if (frequency >= F_CPU/((uint32_t)1<<(16+2)) ) {
        pres = 2;
        prescaler = TC_CLKSEL_DIV2_gc;
    }
    else if (frequency >= F_CPU/((uint32_t)1<<(16+3)) ) {
        pres = 3;
        prescaler = TC_CLKSEL_DIV4_gc;
    }
    else if (frequency >= F_CPU/((uint32_t)1<<(16+4)) ) {
        pres = 4;
        prescaler = TC_CLKSEL_DIV8_gc;
    }
    else if (frequency >= F_CPU/((uint32_t)1<<(16+7)) ) {
        pres = 7;
        prescaler = TC_CLKSEL_DIV64_gc;
    }
    else if (frequency >= F_CPU/((uint32_t)1<<(16+9)) ) {
        pres = 9;
        prescaler = TC_CLKSEL_DIV256_gc;
    }
    else if (frequency >= F_CPU/((uint32_t)1<<(16+11)) ) {
        pres = 11;
        prescaler = TC_CLKSEL_DIV1024_gc;
    }
    counter_.configure(prescaler, F_CPU/( (1<<pres)*frequency ), TC_WGMODE_DSTOP_gc);
    frequency_ = frequency;
}

Servo* ServoController::createServo(Counter::Channel::Index index, PinLocation location)
{
    if (servos_[static_cast<uint8_t>(index)] == nullptr) {
        servos_[static_cast<uint8_t>(index)] = new Servo(counter_.createChannel(location, index), frequency_);
        return servos_[static_cast<uint8_t>(index)].getPtr();
    }
    return nullptr;
}
