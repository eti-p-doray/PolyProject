//
//  Hand.h
//  ArmDevice
//
//  Created by Etienne Pierre-Doray on 2015-02-15.
//
//

#ifndef HAND_H_
#define HAND_H_

#include "ArmKnowledge.h"
#include "Servo/Servo.h"

class Hand {
public:
    Hand();
    
    void write(HandInstructions instructions);
    
private:
    UniquePtr<ServoController> controllers_[6];
    Servo* servos_[18];
};

#endif /* HAND_H_ */
