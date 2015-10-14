/*
 * TWI.cpp
 *
 * Created: 2014-07-16 20:57:55
 *  Author: Etienne
 */ 

#include "TWI.h"
#include "avr/io.h"


TwiMaster::TwiMaster(UniquePtr<TwiController>&& twiController, uint32_t frequency)
{
    twiController_ = static_cast<UniquePtr<TwiController>&&>(twiController);
	twiController_->setMasterEventListenner(this);
	twiController_->module()->CTRL           = 0x00;
	twiController_->module()->MASTER.BAUD    = F_CPU / (2*frequency) - 5;
	twiController_->module()->MASTER.CTRLA   = TWI_MASTER_ENABLE_bm | TWI_MASTER_WIEN_bm | TWI_MASTER_RIEN_bm | TWI_MASTER_INTLVL_HI_gc;
    twiController_->module()->MASTER.CTRLB   = 0;
	twiController_->module()->MASTER.CTRLC   = 0;
	twiController_->module()->MASTER.STATUS  = TWI_MASTER_BUSSTATE_IDLE_gc;
	
	twi_ = twiController_->module();
}

TwiMaster::~TwiMaster()
{
	TWIC.MASTER.CTRLA = 0x00;
    queue_.reset();
}

void TwiMaster::reset()
{
	TWIC.MASTER.CTRLA = 0x00;
	queue_.reset();
	twiController_->module()->MASTER.CTRLA   = TWI_MASTER_ENABLE_bm | TWI_MASTER_WIEN_bm | TWI_MASTER_RIEN_bm | TWI_MASTER_INTLVL_HI_gc;
	twiController_->module()->MASTER.STATUS  = TWI_MASTER_BUSSTATE_IDLE_gc;
}

void TwiMaster::write(uint8_t address, const Buffer<uint8_t>& buffer)
{
    Packet packet(address, buffer);
    push(packet);
}

void TwiMaster::write(uint8_t address, const uint8_t data[], uint8_t lenght)
{
    const Buffer<uint8_t> buffer(const_cast<uint8_t*>(data), lenght);
    write(address, buffer);
}

void TwiMaster::read(uint8_t address, Buffer<uint8_t>& buffer)
{
    Packet packet(address | 0x01, buffer);
    push(packet);
}

void TwiMaster::read(uint8_t address, uint8_t data[], uint8_t lenght)
{
    Buffer<uint8_t> buffer(data, lenght);
    read(address, buffer);
}

void TwiMaster::insertToken(volatile RequestStatus* status)
{
    *status = REQUEST_PENDING;
    Packet token(status);
    push(token);
}

uint8_t TwiMaster::getState() const
{
	return twiController_->module()->MASTER.STATUS;
}

void TwiMaster::onTwiMasterEvent()
{
	TWI_t* twi = twiController_->module();
	if ((twi->MASTER.STATUS & (TWI_MASTER_ARBLOST_bm | TWI_MASTER_BUSERR_bm)) != 0) {
		twi->MASTER.CTRLC |= TWI_MASTER_CMD_STOP_gc;
        queue_.front().setToEnd();
        status_ = REQUEST_FAILED;
	}
    else if ((twi->MASTER.STATUS & (TWI_MASTER_RXACK_bm)) != 0) {
        queue_.front().setToEnd();
        status_ = REQUEST_FAILED;
    }
	else if ((twi->MASTER.STATUS & TWI_MASTER_RIF_bm) != 0) {
		queue_.front().push(twi->MASTER.DATA);
	}
	else if ((twi->MASTER.STATUS & TWI_MASTER_WIF_bm) != 0) {
		twi->MASTER.DATA = queue_.front().pull();
		queue_.front().pop();
	}
	
	if (queue_.front().isAtEnd()) {
		twi->MASTER.CTRLC |= TWI_MASTER_ACKACT_bm;
		queue_.pop();
        while (queue_.isEmpty() == false && queue_.front().isToken()) {
			if (queue_.front().getData().read() != nullptr) {
				if (status_ == REQUEST_PENDING) {
					queue_.front().setToken(REQUEST_DONE);
                }
				else {
					queue_.front().setToken(REQUEST_FAILED);
				}
            }
			queue_.pop();
        }
        if (queue_.isEmpty() == false) {
            status_ = REQUEST_PENDING;
            while ((twi->MASTER.STATUS & TWI_MASTER_BUSSTATE_gm) == TWI_MASTER_BUSSTATE_BUSY_gc);
			twi->MASTER.ADDR = queue_.front().getAddress();
			//twi->MASTER.CTRLC |= TWI_MASTER_CMD_REPSTART_gc;
		}
		else {
			twi->MASTER.CTRLC |= TWI_MASTER_CMD_STOP_gc;
		}
	}
	else {
		twi->MASTER.CTRLC &= ~TWI_MASTER_ACKACT_bm;
		twi->MASTER.CTRLC |= TWI_MASTER_CMD_RECVTRANS_gc;
	}
}
        
void TwiMaster::push(Packet packet)
{
    twiController_->module()->MASTER.CTRLA &= ~TWI_MASTER_INTLVL_gm;
    if ( (twiController_->module()->MASTER.STATUS & TWI_MASTER_BUSSTATE_gm) == TWI_MASTER_BUSSTATE_IDLE_gc && queue_.isEmpty()) {
		status_ = REQUEST_PENDING;
        twiController_->module()->MASTER.ADDR = packet.getAddress();
    }
    queue_.push(packet);
    twiController_->module()->MASTER.CTRLA |= TWI_MASTER_INTLVL_HI_gc;
}

