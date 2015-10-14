//
//  Usart.cpp
//  ArmDevice
//
//  Created by Etienne Pierre-Doray on 2015-02-11.
//
//

#include "Usart.h"

Usart::Usart(UniquePtr<UsartController>&& usartController, uint32_t frequency) : txData_(255), rxData_(255)
{
    usartController_ = static_cast<UniquePtr<UsartController>&&>(usartController);
    
    configure(frequency, USART_CMODE_ASYNCHRONOUS_gc, USART_PMODE_DISABLED_gc, false, USART_CHSIZE_8BIT_gc);
	port = usartController_->getPort();
	usart = usartController_->module();
}

Usart::Usart(UniquePtr<UsartController>&& usartController, uint32_t frequency, USART_CMODE_enum communicationMode, USART_PMODE_enum parityMode, bool stopBitsMode, USART_CHSIZE_enum characterSize) : txData_(255), rxData_(255)
{
    usartController_ = static_cast<UniquePtr<UsartController>&&>(usartController);
    
    configure(frequency, communicationMode, parityMode, stopBitsMode, characterSize);
}

Usart::~Usart()
{
	disableTransmit();
	usartController_->module()->CTRLA = 0x00;
    usartController_->module()->CTRLB = 0x00;
    usartController_->module()->CTRLC = 0x00;
}

void Usart::configure(uint32_t frequency, USART_CMODE_enum communicationMode, USART_PMODE_enum parityMode, bool stopBitsMode, USART_CHSIZE_enum characterSize)
{
    usartController_->module()->CTRLA = 0x00;
    usartController_->module()->CTRLB = 0x00;
    usartController_->module()->CTRLC = communicationMode | parityMode | stopBitsMode | characterSize;
    
    int8_t scale = ceil(log(F_CPU)/M_LN2 - log(frequency)/M_LN2 - 16.0);
    if (scale > 7)
        scale = 7;
    else if (scale < -7)
        scale = -7;
    uint16_t sel;
    if (scale < 0) {
        sel = round( float(1<<(-scale)) * ((float(F_CPU)/ (16.0 * frequency)) - 1.0));
    }
    else {
        sel = round((float(F_CPU) / ( 16.0 * frequency) - 1.0) / float(1<<scale) );
    }
    
    usartController_->module()->BAUDCTRLB = (scale << 4) | (sel >> 8);
    usartController_->module()->BAUDCTRLA = sel & 0xff;
    
}
void Usart::configureSpi(bool dataOrder, bool clockPhase)
{
    usartController_->module()->CTRLC = USART_CMODE_MSPI_gc | dataOrder | clockPhase;
}

void Usart::enableTransmit()
{
    usartController_->setTransmitListenner(this);
	if (usartController_->getType() == 0) {
		usartController_->getPort()->DIR |= (1 << (3 + ((usartController_->getPort()->REMAP&(1<<PR_USART0_bp))!=0)*4) );
	}
	else {
		usartController_->getPort()->DIR |= (1 << 7);
	}
    usartController_->module()->CTRLA |= USART_TXCINTLVL_MED_gc;
    usartController_->module()->CTRLB |= USART_TXEN_bm;
    
}

void Usart::enableReceive()
{
    usartController_->setReceiveListenner(this);
    usartController_->module()->CTRLA |= USART_RXCINTLVL_MED_gc;
    usartController_->module()->CTRLB |= USART_RXEN_bm;
}

void Usart::disableTransmit()
{
	usartController_->getPort()->DIR &= ~(1 << (3 + ((usartController_->getPort()->REMAP&(1<<PR_USART0_bp))!=0)*4) );
    usartController_->module()->CTRLA &= ~USART_TXCINTLVL_MED_gc;
    usartController_->module()->CTRLB &= ~USART_TXEN_bm;
}

void Usart::disableReceive()
{
    usartController_->module()->CTRLA &= ~USART_RXCINTLVL_MED_gc;
    usartController_->module()->CTRLB &= ~USART_RXEN_bm;
}

void Usart::map(PinLocation location)
{
    if (usartController_->getType() != 0) {
        return;
    }
	bool transmitEnabled = (usartController_->module()->CTRLB & USART_TXEN_bm) != 0; 
	usartController_->getPort()->DIR &= ~(transmitEnabled << (3 + ((usartController_->getPort()->REMAP&(1<<PR_USART0_bp))!=0)*4) );
    usartController_->getPort()->REMAP  = (usartController_->getPort()->REMAP & ~(PORT_USART0_bm) ) | (static_cast<uint8_t>(location) << PR_USART0_bp);
	usartController_->getPort()->DIR |= (transmitEnabled << (3 + ((usartController_->getPort()->REMAP&(1<<PR_USART0_bp))!=0)*4) );
}

bool Usart::dataAvailable() const
{
    return !rxData_.isEmpty();
}

void Usart::write(uint8_t data)
{
    while (txData_.isFull());
    usartController_->module()->CTRLA &= ~USART_TXCINTLVL_gm;
    if (txData_.isEmpty() && (usartController_->module()->STATUS & USART_DREIF_bm) != 0)
    {
        usartController_->module()->DATA = data;
    }
    else {
        txData_.write(data);
    }
    usartController_->module()->CTRLA |= USART_TXCINTLVL_MED_gc;
}

uint8_t Usart::read()
{
    return rxData_.read();
}

void Usart::onUsartTransmit()
{
	if (!txData_.isEmpty()) {
		usartController_->module()->DATA = txData_.read();
	}
}

void Usart::onUsartReceive()
{
    rxData_.write(usartController_->module()->DATA);
}