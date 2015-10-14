/*
 * Packet.cpp
 *
 * Created: 2014-08-11 19:51:47
 *  Author: Etienne
 */ 

#include "Twi.h"

TwiMaster::Packet::Packet(uint8_t address, Buffer<uint8_t> buffer)
{
    index_ = 0;
    buffer_ = buffer;
    address_ = address;
}

TwiMaster::Packet::Packet(volatile RequestStatus* status)
{
    index_ = 0;
    buffer_ = Buffer<uint8_t>((uint8_t*)status, 1);
    address_ = TOKEN_ADDRESS;
}

bool TwiMaster::Packet::isToken() const
{
    return address_ == TOKEN_ADDRESS;
}

void TwiMaster::Packet::push(uint8_t data) {
    buffer_.write(data, index_);
    index_ ++;
}