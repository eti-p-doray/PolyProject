//
//  Packet.cpp
//  ArmDevice
//
//  Created by Etienne Pierre-Doray on 2015-02-13.
//
//

#include "XBee.h"

XBee::Packet::Packet()
{
    index_ = -3;
    checksum_ = 0;
}

bool XBee::Packet::isAtEnd() const
{
    return index_ == lenght_ + 1;
}

void XBee::Packet::reset()
{
	index_ = -3;
	checksum_ = 0;
	lenght_ = 0;
}

XBee::RequestPacket::RequestPacket(XBee::RequestFrameData* frameData) : Packet()
{
    frameData_ = frameData;
    lenght_ = frameData_->lenght();
}

uint8_t XBee::RequestPacket::read()
{
    uint8_t data = 0;
    if (index_ == -3) {
        data = START_DELIMITER;
    }
    else if (index_ == -2) {
        data = frameData_->lenght() >> 8;
    }
    else if (index_ == -1) {
        data = frameData_->lenght();
    }
    else if (index_ >= 0 && index_ < lenght_) {
        data = frameData_->read(index_);
        checksum_ += data;
    }
    else if (index_ == lenght_) {
        data = 0xff - checksum_;
    }
    index_ ++;
    return data;
}

XBee::ResponsePacket::ResponsePacket(uint16_t bufferLenght) : Packet(), buffer_(bufferLenght)
{
    frameData_ = new InvalidFrameData();
    lenght_ = 0;
}

void XBee::ResponsePacket::write(uint8_t data)
{
    if (index_ == -3) {
        if (data != START_DELIMITER) {
            index_ = 0;
        }
    }
    else if (index_ == -2) {
        lenght_ = data << 8;
    }
    else if (index_ == -1) {
        lenght_ |= data;
    }
    else if (index_ == 0) {
        switch (static_cast<Type>(data)) {
			case Type::TRANSMIT_STATUS:
				frameData_ = new TransmitStatus;
				break;
			
            case Type::ZB_TRANSMIT_STATUS:
                frameData_ = new ZbTransmitStatus();
                break;
				
			case Type::RECEIVE64:
				frameData_ = new Receive64(Buffer<uint8_t>(buffer_.read(), lenght_-11));
				break;
				
			case Type::RECEIVE16:
				frameData_ = new Receive16(Buffer<uint8_t>(buffer_.read(), lenght_-5));
				break;
                
            case Type::ZB_RECEIVE:
                frameData_ = new ZbReceive(Buffer<uint8_t>(buffer_.read(), lenght_-12));
                break;
                
            default:
				frameData_ = new InvalidFrameData;
                index_ = lenght_;
                break;
        }
        checksum_ += data;
    }
    else if (index_ > 0 && index_ < lenght_) {
        frameData_->write(data, index_);
        checksum_ += data;
    }
    else if (index_ == lenght_) {
        checksum_ += data;
        if (checksum_ != 0xff) {
            frameData_ = new InvalidFrameData();
        }
    }
    index_ ++;
}

bool XBee::ResponsePacket::isValid() const
{
	return (isAtEnd() && frameData_->getType() != Type::INVALID);
}

UniquePtr<XBee::ResponseFrameData> XBee::ResponsePacket::getFrame()
{
	UniquePtr<ResponseFrameData> frameData = static_cast<UniquePtr<ResponseFrameData>&&>(frameData_);
	frameData_ = new InvalidFrameData;
    return static_cast<UniquePtr<ResponseFrameData>&&>(frameData);
}