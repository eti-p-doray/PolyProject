//
//  Usart.h
//  ArmDevice
//
//  Created by Etienne Pierre-Doray on 2015-02-11.
//
//

#ifndef USART_H_
#define USART_H_

#include "math.h"
#include "../Controller/ModuleController.h"
#include "../DataStructure/CircularBuffer.h"
#include "../DataStructure/UniquePtr.h"

class Usart : public UsartInterrupt::ReceiveListenner, public UsartInterrupt::TransmitListenner {
public:
    Usart(UniquePtr<UsartController>&& usartController, uint32_t frequency);
    Usart(UniquePtr<UsartController>&& usartController, uint32_t frequency, USART_CMODE_enum communicationMode, USART_PMODE_enum parityMode, bool stopBitsMode, USART_CHSIZE_enum characterSize);
    ~Usart();
	
    void configure(uint32_t frequency, USART_CMODE_enum communicationMode, USART_PMODE_enum parityMode, bool stopBitsMode, USART_CHSIZE_enum characterSize);
    void configureSpi(bool dataOrder, bool clockPhase);
    void enableTransmit();
    void enableReceive();
	void disableTransmit();
    void disableReceive();
    void map(PinLocation location);
    
    bool dataAvailable() const;
    void write(uint8_t data);
    uint8_t read();
    
protected:
    void onUsartTransmit();
    void onUsartReceive();
    
private:
    UniquePtr<UsartController> usartController_;
    volatile CircularBuffer<uint8_t> txData_;
    volatile CircularBuffer<uint8_t> rxData_;
	
	PORT_t* port;
	USART_t* usart;
};

#endif
