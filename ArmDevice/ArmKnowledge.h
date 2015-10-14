//
//  ArmKnowledge.h
//  ArmDevice
//
//  Created by Etienne Pierre-Doray on 2015-02-15.
//
//

#ifndef ArmDevice_ArmKnowledge_h
#define ArmDevice_ArmKnowledge_h

#include <stdlib.h>
#include <stdint.h>

struct JointInstructions {
    int16_t position = 0;
};
struct FingerInstructions {
    JointInstructions joints[4];
};
struct ForearmInstructions {
    JointInstructions wristFlexion[2];
};
struct HandInstructions {
    ForearmInstructions forearm;
    FingerInstructions fingers[5];
};


struct ImuObservationsBare {
    int16_t accl[3] = {};
    int16_t magn[3] = {};
    int16_t gyro[3] = {};
};

#endif
