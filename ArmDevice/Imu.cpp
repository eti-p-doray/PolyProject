//
//  Imu.cpp
//  ArmDevice
//
//  Created by Etienne Pierre-Doray on 2015-02-14.
//
//

#include "Imu.h"

const uint8_t magnAddress = 0x3C;
const uint8_t acclAddress = 0x30;
const uint8_t gyroAddress = 0xD4;

const uint8_t ctrlReg1Accl[] = {0x20, 0x57}; //enable, 100hz
const uint8_t ctrlReg4Accl[] = {0x23, 0x08}; //little-endian, high resolution
	
const uint8_t magnCtrlA[] = {0x00, 0x98}; // temperature sensor, 75Hz
const uint8_t magnCtrlB[] = {0x01, 0x00}; //0.8Ga
const uint8_t magnMode[] = {0x02, 0x00}; //Continuous measurement
	
const uint8_t ctrlReg1Gyro[] = {0x20, 0x0F}; //95hz, cutt-off 12.5, enable
const uint8_t ctrlReg4Gyro[] = {0x23, 0x00}; //little-endian, 250dps
	
const uint8_t readPtrAccl[] = {0xA8};
const uint8_t readPtrGyro[] = {0xA8};
const uint8_t readPtrMagn[] = {0x03};


Imu::Imu() : twi_(TwiController::instance(TwiController::Index::TWIC_IDX), 400000)
{
    init();
}

bool Imu::read(ImuObservationsBare& observation)
{
	if (status_ == RequestStatus::REQUEST_PENDING) {
		return false;
	}
    
	status_ = RequestStatus::REQUEST_PENDING;
	dataRead = false;
	
    twi_.write(acclAddress, readPtrAccl, sizeof(readPtrAccl));
	twi_.read(acclAddress, (uint8_t*)&observation, 3*sizeof(int16_t));
	
	twi_.write(gyroAddress, readPtrGyro, sizeof(readPtrGyro));
    twi_.read(gyroAddress, (uint8_t*)&observation+6*sizeof(int16_t), 3*sizeof(int16_t));
    
    twi_.write(magnAddress, readPtrMagn, sizeof(readPtrMagn));
    twi_.read(magnAddress, (uint8_t*)&observation+3*sizeof(int16_t), 3*sizeof(int16_t));
    
    twi_.insertToken(&status_);
    return true;
}

RequestStatus Imu::getStatus() const
{
    return status_;
}

bool Imu::isReady() const
{
    return (status_ == RequestStatus::REQUEST_DONE || status_ == REQUEST_FAILED) && (dataRead == false);
}

void Imu::clear()
{
    status_ = RequestStatus::REQUEST_DONE;
	dataRead = true;
}

void Imu::reset()
{
	twi_.reset();
    init();
}

void Imu::init()
{
	status_ = RequestStatus::REQUEST_DONE;
	dataRead = false;
	
    twi_.write(acclAddress, ctrlReg1Accl, sizeof(ctrlReg1Accl));
	twi_.write(acclAddress, ctrlReg4Accl, sizeof(ctrlReg4Accl));
	
	twi_.write(gyroAddress, ctrlReg1Gyro, sizeof(ctrlReg1Gyro));
    twi_.write(gyroAddress, ctrlReg4Gyro, sizeof(ctrlReg4Gyro));
    
	twi_.write(magnAddress, magnCtrlA, sizeof(magnCtrlA));
    twi_.write(magnAddress, magnCtrlB, sizeof(magnCtrlB));
    twi_.write(magnAddress, magnMode, sizeof(magnMode));
}