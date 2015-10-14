/*
 * XBee.cpp
 *
 * Created: 2014-08-24 08:40:30
 *  Author: Etienne
 */ 

#include "XBee.h"

XBee::XBee(UniquePtr<UsartController>&& usartController) : usart_(static_cast<UniquePtr<UsartController>&&>(usartController), 57600)
{
    usart_.enableTransmit();
    usart_.enableReceive();
}

XBee::~XBee()
{
    
}

void XBee::read(XBee::ResponsePacket* packet)
{
	if (packet->isAtEnd()) {
		packet->reset();
	}
    while (!packet->isAtEnd() && usart_.dataAvailable()) {
        packet->write(usart_.read());
    }
}

void XBee::write(XBee::RequestPacket packet)
{
    while (!packet.isAtEnd()) {
        usart_.write(packet.read());
    }
}

void XBee::map(PinLocation location)
{
	usart_.map(location);
}


