/*Auto generated code*/

#ifndef MODULE_CONTROLLER_H_
#define MODULE_CONTROLLER_H_

#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include "InterruptVector.h"
#include "InterruptController.h"
#include "../DataStructure/uniquePtr.h"

enum class PinLocation : bool {NORMAL, REMAP};

class UsartController {
	friend void USARTC0_RXC_vect(void);
	friend void USARTC0_DRE_vect(void);
	friend void USARTC0_TXC_vect(void);
	friend void USARTC1_RXC_vect(void);
	friend void USARTC1_DRE_vect(void);
	friend void USARTC1_TXC_vect(void);
	friend void USARTD0_RXC_vect(void);
	friend void USARTD0_DRE_vect(void);
	friend void USARTD0_TXC_vect(void);
	friend void USARTD1_RXC_vect(void);
	friend void USARTD1_DRE_vect(void);
	friend void USARTD1_TXC_vect(void);
	friend void USARTE0_RXC_vect(void);
	friend void USARTE0_DRE_vect(void);
	friend void USARTE0_TXC_vect(void);
	friend void USARTE1_RXC_vect(void);
	friend void USARTE1_DRE_vect(void);
	friend void USARTE1_TXC_vect(void);
	friend void USARTF0_RXC_vect(void);
	friend void USARTF0_DRE_vect(void);
	friend void USARTF0_TXC_vect(void);
	friend void USARTF1_RXC_vect(void);
	friend void USARTF1_DRE_vect(void);
	friend void USARTF1_TXC_vect(void);

public:
	enum class Index {USARTC0_IDX, USARTC1_IDX, USARTD0_IDX, USARTD1_IDX, USARTE0_IDX, USARTE1_IDX, USARTF0_IDX, USARTF1_IDX, COUNT};
	static USART_t* Modules[static_cast<uint8_t>(Index::COUNT)];
	static PORT_t* Ports[static_cast<uint8_t>(Index::COUNT)];
	static const uint8_t Type[static_cast<uint8_t>(Index::COUNT)];

	~UsartController() {*instance_ = nullptr;}

	USART_t* module() {return Modules[static_cast<uint8_t>(index_)];}
	const USART_t* module() const {return Modules[static_cast<uint8_t>(index_)];}
	PORT_t* getPort() const {return Ports[static_cast<uint8_t>(index_)];}
	uint8_t getType() const {return Type[static_cast<uint8_t>(index_)];}
	Index getIndex() const {return index_;}
	void setReceiveListenner(UsartInterrupt::ReceiveListenner* listenner) {interruptVector_.Receive = listenner;}
	void setDataRegisterEmptyListenner(UsartInterrupt::DataRegisterEmptyListenner* listenner) {interruptVector_.DataRegisterEmpty = listenner;}
	void setTransmitListenner(UsartInterrupt::TransmitListenner* listenner) {interruptVector_.Transmit = listenner;}
	static UniquePtr<UsartController> instance(Index index);

private:
	UsartController(Index index, UsartController** instance) {
		index_ = index;
		instance_ = instance;
	}
	UsartController(const UsartController&);
	UsartController& operator = (const UsartController&);

	static UsartController* instances_[static_cast<uint8_t>(UsartController::Index::COUNT)];
	UsartController** instance_;
	Index index_;
	InterruptController interrupt_;
	UsartInterrupt interruptVector_;
};

class CounterController {
	friend void TCC0_OVF_vect(void);
	friend void TCC0_ERR_vect(void);
	friend void TCC0_CCA_vect(void);
	friend void TCC0_CCB_vect(void);
	friend void TCC0_CCC_vect(void);
	friend void TCC0_CCD_vect(void);
	friend void TCD0_OVF_vect(void);
	friend void TCD0_ERR_vect(void);
	friend void TCD0_CCA_vect(void);
	friend void TCD0_CCB_vect(void);
	friend void TCD0_CCC_vect(void);
	friend void TCD0_CCD_vect(void);
	friend void TCE0_OVF_vect(void);
	friend void TCE0_ERR_vect(void);
	friend void TCE0_CCA_vect(void);
	friend void TCE0_CCB_vect(void);
	friend void TCE0_CCC_vect(void);
	friend void TCE0_CCD_vect(void);
	friend void TCF0_OVF_vect(void);
	friend void TCF0_ERR_vect(void);
	friend void TCF0_CCA_vect(void);
	friend void TCF0_CCB_vect(void);
	friend void TCF0_CCC_vect(void);
	friend void TCF0_CCD_vect(void);
	friend void TCC1_OVF_vect(void);
	friend void TCC1_ERR_vect(void);
	friend void TCC1_CCA_vect(void);
	friend void TCC1_CCB_vect(void);
	friend void TCC1_CCC_vect(void);
	friend void TCC1_CCD_vect(void);
	friend void TCD1_OVF_vect(void);
	friend void TCD1_ERR_vect(void);
	friend void TCD1_CCA_vect(void);
	friend void TCD1_CCB_vect(void);
	friend void TCD1_CCC_vect(void);
	friend void TCD1_CCD_vect(void);
	friend void TCE1_OVF_vect(void);
	friend void TCE1_ERR_vect(void);
	friend void TCE1_CCA_vect(void);
	friend void TCE1_CCB_vect(void);
	friend void TCE1_CCC_vect(void);
	friend void TCE1_CCD_vect(void);
	friend void TCF1_OVF_vect(void);
	friend void TCF1_ERR_vect(void);
	friend void TCF1_CCA_vect(void);
	friend void TCF1_CCB_vect(void);
	friend void TCF1_CCC_vect(void);
	friend void TCF1_CCD_vect(void);

public:
	enum class Index {TCC0_IDX, TCD0_IDX, TCE0_IDX, TCF0_IDX, TCC1_IDX, TCD1_IDX, TCE1_IDX, TCF1_IDX, COUNT};
	static TC0_t* Modules[static_cast<uint8_t>(Index::COUNT)];
	static PORT_t* Ports[static_cast<uint8_t>(Index::COUNT)];
	static const uint8_t Type[static_cast<uint8_t>(Index::COUNT)];

	~CounterController() {*instance_ = nullptr;}

	TC0_t* module() {return Modules[static_cast<uint8_t>(index_)];}
	const TC0_t* module() const {return Modules[static_cast<uint8_t>(index_)];}
	PORT_t* getPort() const {return Ports[static_cast<uint8_t>(index_)];}
	uint8_t getType() const {return Type[static_cast<uint8_t>(index_)];}
	Index getIndex() const {return index_;}
	void setOverflowListenner(CounterInterrupt::OverflowListenner* listenner) {interruptVector_.Overflow = listenner;}
	void setErrorListenner(CounterInterrupt::ErrorListenner* listenner) {interruptVector_.Error = listenner;}
	void setChannelListenner(CounterInterrupt::ChannelListenner* listenner, int i) {interruptVector_.Channel[i] = listenner;}
	static UniquePtr<CounterController> instance(Index index);

private:
	CounterController(Index index, CounterController** instance) {
		index_ = index;
		instance_ = instance;
	}
	CounterController(const CounterController&);
	CounterController& operator = (const CounterController&);

	static CounterController* instances_[static_cast<uint8_t>(CounterController::Index::COUNT)];
	CounterController** instance_;
	Index index_;
	InterruptController interrupt_;
	CounterInterrupt interruptVector_;
};

class TwiController {
	friend void TWIC_TWIS_vect(void);
	friend void TWIC_TWIM_vect(void);
	friend void TWID_TWIS_vect(void);
	friend void TWID_TWIM_vect(void);
	friend void TWIE_TWIS_vect(void);
	friend void TWIE_TWIM_vect(void);
	friend void TWIF_TWIS_vect(void);
	friend void TWIF_TWIM_vect(void);

public:
	enum class Index {TWIC_IDX, TWID_IDX, TWIE_IDX, TWIF_IDX, COUNT};
	static TWI_t* Modules[static_cast<uint8_t>(Index::COUNT)];
	~TwiController() {*instance_ = nullptr;}

	TWI_t* module() {return Modules[static_cast<uint8_t>(index_)];}
	const TWI_t* module() const {return Modules[static_cast<uint8_t>(index_)];}
	Index getIndex() const {return index_;}
	void setSlaveEventListenner(TwiInterrupt::SlaveEventListenner* listenner) {interruptVector_.SlaveEvent = listenner;}
	void setMasterEventListenner(TwiInterrupt::MasterEventListenner* listenner) {interruptVector_.MasterEvent = listenner;}
	static UniquePtr<TwiController> instance(Index index);

private:
	TwiController(Index index, TwiController** instance) {
		index_ = index;
		instance_ = instance;
	}
	TwiController(const TwiController&);
	TwiController& operator = (const TwiController&);

	static TwiController* instances_[static_cast<uint8_t>(TwiController::Index::COUNT)];
	TwiController** instance_;
	Index index_;
	InterruptController interrupt_;
	TwiInterrupt interruptVector_;
};

class PortController {

public:
	enum class Index {PORTA_IDX, PORTB_IDX, PORTC_IDX, PORTD_IDX, PORTE_IDX, PORTF_IDX, PORTH_IDX, PORTJ_IDX, PORTK_IDX, PORTQ_IDX, COUNT};
	static PORT_t* Modules[static_cast<uint8_t>(Index::COUNT)];
	~PortController() {*instance_ = nullptr;}

	PORT_t* module() {return Modules[static_cast<uint8_t>(index_)];}
	const PORT_t* module() const {return Modules[static_cast<uint8_t>(index_)];}
	Index getIndex() const {return index_;}
	static UniquePtr<PortController> instance(Index index);

private:
	PortController(Index index, PortController** instance) {
		index_ = index;
		instance_ = instance;
	}
	PortController(const PortController&);
	PortController& operator = (const PortController&);

	static PortController* instances_[static_cast<uint8_t>(PortController::Index::COUNT)];
	PortController** instance_;
	Index index_;
	InterruptController interrupt_;
	PortInterrupt interruptVector_;
};

#endif /* MODULE_CONTROLLER_H_ */
