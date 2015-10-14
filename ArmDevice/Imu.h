//
//  Imu.h
//  ArmDevice
//
//  Created by Etienne Pierre-Doray on 2015-02-14.
//
//

#ifndef IMU_H_
#define IMU_H_

#include "DataStructure/Buffer.h"
#include "ArmKnowledge.h"
#include "Twi/Twi.h"

class Imu {
public:
    Imu();
    
    bool read(ImuObservationsBare& observation);
    RequestStatus getStatus() const;
    bool isReady() const;
    void clear();
    void reset();

private:
    void init();
    
    volatile RequestStatus status_;
	bool dataRead;
    TwiMaster twi_;
};

#endif /* SENSOR_READER_H_ */
