#include "xbee.h"

XBee::Packet::Packet()
{
    index_ = -3;
    checksum_ = 0;
    lenght_ = 0;
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

XBee::RequestPacket::RequestPacket(XBee::RequestFrameData* frameData)
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

XBee::ResponsePacket::ResponsePacket(uint16_t bufferLenght) : buffer_(bufferLenght, 0)
{
    frameData_.reset(new InvalidFrameData());
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
            frameData_.reset(new TransmitStatus());
            break;

        case Type::ZB_TRANSMIT_STATUS:
            frameData_.reset(new ZbTransmitStatus());
            break;
            
        case Type::RECEIVE64:
            frameData_.reset(new Receive64(Buffer<uint8_t>((uint8_t*)buffer_.data(), lenght_-11)));
            break;
            
        case Type::RECEIVE16:
            frameData_.reset(new Receive16(Buffer<uint8_t>((uint8_t*)buffer_.data(), lenght_-5)));
            break;
            
        case Type::ZB_RECEIVE:
            frameData_.reset(new ZbReceive(Buffer<uint8_t>((uint8_t*)buffer_.data(), lenght_-12)));
            break;
            
        default:
            frameData_.reset(new InvalidFrameData);
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
            frameData_.reset(new InvalidFrameData());
        }
    }
    index_ ++;
}

bool XBee::ResponsePacket::isValid() const
{
    return (isAtEnd() && frameData_->getType() != Type::INVALID);
}

void XBee::ResponsePacket::getFrame(QScopedPointer<XBee::ResponseFrameData>& responseFrameData)
{
    responseFrameData.reset(new InvalidFrameData);
    frameData_.swap(responseFrameData);
}
