#include "handDevice.h"

HandDevice::HandDevice(XBee* xbee) : xbee_(xbee), responsePacket_(sizeof(ImuObservationsBare))
{
    xbeeReception_ = connect(xbee_->getPort(), &QSerialPort::readyRead, this, &HandDevice::onReadyRead);
}

HandDevice::~HandDevice()
{
    QObject::disconnect(xbeeReception_);
}

void HandDevice::sendInstructions(HandInstructions instructions)
{
    instructions_ = instructions;
    //XBee::ZbTransmitRequest request(0x0013a200408cd2d2, 0xfffe, (uint8_t*)&instructions_, sizeof(HandInstructions));
    //XBee::Transmit64Request request(0x0013a200403B3DD2, (uint8_t*)&instructions_, sizeof(HandInstructions));
    XBee::Transmit16Request request(0x00, (uint8_t*)&instructions_, sizeof(HandInstructions));
    //request.setId(1);
    xbee_->write(&request);
}

void HandDevice::onReadyRead()
{
    xbee_->read(&responsePacket_);
    while (responsePacket_.isValid()) {
        if (responsePacket_.getFrameType() == XBee::Type::RECEIVE16) {
            emit observationsReceive(responsePacket_.getFrame<XBee::Receive16>().getData<ImuObservationsBare>());
        }
        xbee_->read(&responsePacket_);
    }
}
