 //
//  FrameData.cpp
//  ArmDevice
//
//  Created by Etienne Pierre-Doray on 2015-02-13.
//
//

#include "XBee.h"

XBee::Transmit64Request::Transmit64Request(uint64_t destinationAddress, Buffer<uint8_t> buffer)
{
	id_ = 0;
	destinationAddress_ = destinationAddress;
	options_ = 0;
	buffer_ = buffer;
}

XBee::Transmit64Request::Transmit64Request(uint64_t destinationAddress, uint8_t* data, uint16_t lenght)
{
	id_ = 0;
	destinationAddress_ = destinationAddress;
	options_ = 0;
	buffer_ = Buffer<uint8_t>(data, lenght);
}
	
uint8_t XBee::Transmit64Request::read(uint16_t index) const
{
	if (index == 0) {
		return static_cast<uint8_t>(getType());
	}
	else if (index == 1) {
		return id_;
	}
	else if (index >= 2 && index < 10) {
		return destinationAddress_ >> ( (9-index) * 8 );
	}
	else if (index == 10) {
		return options_;
	}
	else if (index >= 11 && index < 11+buffer_.lenght()) {
		return buffer_.read(index - 11);
	}
	else {
		return 0;
	}
}
	
XBee::Transmit16Request::Transmit16Request(uint16_t destinationAddress, Buffer<uint8_t> buffer)
{
	id_ = 0;
	destinationAddress_ = destinationAddress;
	options_ = 0;
	buffer_ = buffer;
}

XBee::Transmit16Request::Transmit16Request(uint16_t destinationAddress, uint8_t* data, uint16_t lenght)
{
	id_ = 0;
	destinationAddress_ = destinationAddress;
	options_ = 0;
	buffer_ = Buffer<uint8_t>(data, lenght);
}

uint8_t XBee::Transmit16Request::read(uint16_t index) const
{
	if (index == 0) {
		return static_cast<uint8_t>(getType());
	}
	else if (index == 1) {
		return id_;
	}
	else if (index >= 2 && index < 4) {
		return destinationAddress_ >> ( (3-index) * 8 );
	}
	else if (index == 4) {
		return options_;
	}
	else if (index >= 5 && index < 5+buffer_.lenght()) {
		return buffer_.read(index - 5);
	}
	else {
		return 0;
	}
}

XBee::TransmitStatus::TransmitStatus()
{
	id_ = 0;
	status_ = 0;	
}

void XBee::TransmitStatus::write(uint8_t data, uint16_t index)
{
	if (index == 1) {
		id_ = data;
	}
	else if (index == 2) {
		status_ = data;
	}
}

XBee::ZbTransmitRequest::ZbTransmitRequest(uint64_t destinationAddress, uint16_t destinationNetwork, Buffer<uint8_t> buffer)
{
    id_ = 0;
    destinationAddress_ = destinationAddress;
    destinationNetwork_ = destinationNetwork;
    broadcastRadius_ = 0;
    options_ = 0;
    buffer_ = buffer;
}

XBee::ZbTransmitRequest::ZbTransmitRequest(uint64_t destinationAddress, uint16_t destinationNetwork, uint8_t* data, uint16_t lenght)
{
    id_ = 0;
    destinationAddress_ = destinationAddress;
    destinationNetwork_ = destinationNetwork;
    broadcastRadius_ = 0;
    options_ = 0;
    buffer_ = Buffer<uint8_t>(data, lenght);
}

uint8_t XBee::ZbTransmitRequest::read(uint16_t index) const
{
	if (index == 0) {
		return static_cast<uint8_t>(getType());
	}
    else if (index == 1) {
        return id_;
    }
    else if (index >= 2 && index < 10) {
        return destinationAddress_ >> ( (9-index) * 8 );
    }
    else if (index >= 10 && index < 12) {
        return destinationNetwork_ >> ( (11-index) * 8 );
    }
    else if (index == 12) {
        return broadcastRadius_;
    }
    else if (index == 13) {
        return options_;
    }
    else if (index >= 14 && index < 14+buffer_.lenght()) {
        return buffer_.read(index - 14);
    }
    else {
        return 0;
    }
}

XBee::ZbTransmitStatus::ZbTransmitStatus()
{
    id_ = 0;
    destinationNetwork_ = 0;
    retryCount_ = 0;
    deliveryStatus_ = 0;
    discoveryStatus_ = 0;
}

void XBee::ZbTransmitStatus::write(uint8_t data, uint16_t index)
{
    if (index == 1) {
        id_ = data;
    }
    else if (index >= 2 && index < 4) {
        destinationNetwork_ |= data << ( (3-index) * 8 );
    }
    else if (index == 4) {
        retryCount_ = data;
    }
    else if (index == 5) {
        deliveryStatus_ = data;
    }
    else if (index == 6) {
        discoveryStatus_ = data;
    }
}

XBee::Receive64::Receive64(Buffer<uint8_t> buffer)
{
	sourceAddress_ = 0;
	rssi_ = 0;
	options_ = 0;
	
	buffer_ = buffer;
}
	
void XBee::Receive64::write(uint8_t data, uint16_t index)
{
	if (index >= 1 && index < 9) {
		sourceAddress_ |= data << ( (8-index) * 8 );
	}
	else if (index == 9) {
		rssi_ = data;
	}
	else if (index == 10) {
		options_ = data;
	}
	else if (index >= 11 && index < buffer_.lenght()+11) {
		buffer_.write(data, index-11);
	}
}

XBee::Receive16::Receive16(Buffer<uint8_t> buffer)
{
	sourceAddress_ = 0;
	rssi_ = 0;
	options_ = 0;
	
	buffer_ = buffer;
}
	
void XBee::Receive16::write(uint8_t data, uint16_t index)
{
	if (index >= 1 && index < 3) {
		sourceAddress_ |= data << ( (2-index) * 8 );
	}
	else if (index == 3) {
		rssi_ = data;
	}
	else if (index == 4) {
		options_ = data;
	}
	else if (index >= 5 && index < buffer_.lenght()+5) {
		buffer_.write(data, index-5);
	}
}

XBee::ZbReceive::ZbReceive(Buffer<uint8_t> buffer)
{
    sourceAddress_ = 0;
    sourceNetwork_ = 0;
    options_ = 0;
    
    buffer_ = buffer;
}

void XBee::ZbReceive::write(uint8_t data, uint16_t index)
{
    if (index >= 1 && index < 9) {
        sourceAddress_ |= data << ( (8-index) * 8 );
    }
    else if (index >= 9 && index < 11) {
        sourceNetwork_ |= data << ( (10-index) * 8 );
    }
    else if (index == 11) {
        options_ = data;
    }
    else if (index >= 12 && index < buffer_.lenght()+12) {
        buffer_.write(data, index-12);
    }
}
