//
//  Hand.cpp
//  ArmDevice
//
//  Created by Etienne Pierre-Doray on 2015-02-15.
//
//

#include "Hand.h"

Hand::Hand()
{
    controllers_[0] = new ServoController(CounterController::instance(CounterController::Index::TCC0_IDX), 300);
	controllers_[1] = new ServoController(CounterController::instance(CounterController::Index::TCD0_IDX), 300);
	controllers_[2] = new ServoController(CounterController::instance(CounterController::Index::TCD1_IDX), 300);
	controllers_[3] = new ServoController(CounterController::instance(CounterController::Index::TCE0_IDX), 300);
	controllers_[4] = new ServoController(CounterController::instance(CounterController::Index::TCE1_IDX), 300);
	controllers_[5] = new ServoController(CounterController::instance(CounterController::Index::TCF0_IDX), 300);
	//controllers_[6] = new ServoController(CounterController::instance(CounterController::Index::TCF1_IDX), 300);
    
    servos_[0] = controllers_[0]->createServo(Counter::Channel::Index::C, PinLocation::NORMAL);
    servos_[1] = controllers_[0]->createServo(Counter::Channel::Index::D, PinLocation::NORMAL);
    servos_[2] = controllers_[0]->createServo(Counter::Channel::Index::A, PinLocation::REMAP);
    servos_[3] = controllers_[0]->createServo(Counter::Channel::Index::B, PinLocation::REMAP);
    
    servos_[4] = controllers_[1]->createServo(Counter::Channel::Index::A, PinLocation::NORMAL);
    servos_[5] = controllers_[1]->createServo(Counter::Channel::Index::B, PinLocation::NORMAL);
    servos_[6] = controllers_[2]->createServo(Counter::Channel::Index::C, PinLocation::NORMAL);
    servos_[7] = controllers_[2]->createServo(Counter::Channel::Index::D, PinLocation::NORMAL);
    
    servos_[8] = controllers_[2]->createServo(Counter::Channel::Index::A, PinLocation::NORMAL);
    servos_[9] = controllers_[2]->createServo(Counter::Channel::Index::B, PinLocation::NORMAL);
    
    servos_[10] = controllers_[3]->createServo(Counter::Channel::Index::A, PinLocation::NORMAL);
    servos_[11] = controllers_[3]->createServo(Counter::Channel::Index::B, PinLocation::NORMAL);
    servos_[12] = controllers_[3]->createServo(Counter::Channel::Index::C, PinLocation::NORMAL);
    servos_[13] = controllers_[3]->createServo(Counter::Channel::Index::D, PinLocation::NORMAL);
    
    servos_[14] = controllers_[4]->createServo(Counter::Channel::Index::A, PinLocation::NORMAL);
    servos_[15] = controllers_[4]->createServo(Counter::Channel::Index::B, PinLocation::NORMAL);
    
    servos_[16] = controllers_[5]->createServo(Counter::Channel::Index::A, PinLocation::NORMAL);
    servos_[17] = controllers_[5]->createServo(Counter::Channel::Index::B, PinLocation::NORMAL);
    //servos_[18] = controllers_[6]->createServo(Counter::Channel::Index::C, PinLocation::NORMAL);
    //servos_[19] = controllers_[6]->createServo(Counter::Channel::Index::D, PinLocation::NORMAL);
    
    //servos_[20] = controllers_[7]->createServo(Counter::Channel::Index::A, PinLocation::NORMAL);
    //servos_[21] = controllers_[7]->createServo(Counter::Channel::Index::B, PinLocation::NORMAL);
    
    for (int i = 0; i < 18; i++) {
        servos_[i]->setRange(900, 2100);
		servos_[i]->write(0x0000);
    }
}

void Hand::write(HandInstructions instructions)
{
	servos_[0]->write(instructions.fingers[0].joints[3].position);
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 3; j++) {
            servos_[i*3+j+1]->write(instructions.fingers[i].joints[j].position);
        }
    }
	servos_[16]->write(instructions.forearm.wristFlexion[0].position);
	servos_[17]->write(instructions.forearm.wristFlexion[1].position);
}