//
//  Servo.h
//  ArmDevice
//
//  Created by Etienne Pierre-Doray on 2015-02-09.
//
//

#ifndef SERVO_H_
#define SERVO_H_

#include "../Controller/ModuleController.h"
#include "../Counter/Counter.h"


class Servo {
    friend class ServoController;
public:
    void write(int16_t value);
    
    void map(PinLocation location);
    void setRange(uint16_t minPulse, uint16_t maxPulse);
    
private:
    Servo(Counter::Channel* channel, uint16_t frequency);
    ~Servo() = default;
    
    Counter::Channel* channel_;
    uint16_t frequency_;
    uint16_t scale_;
    uint16_t delta_;
};

class ServoController {
public:
    ServoController(UniquePtr<CounterController>&& counterController, uint16_t frequency);
    
    Servo* createServo(Counter::Channel::Index index, PinLocation location);
    
private:
    Counter counter_;
    uint16_t frequency_;
    UniquePtr<Servo> servos_[static_cast<uint8_t>(Counter::Channel::Index::COUNT)];
};

#endif /* SERVO_H_ */
