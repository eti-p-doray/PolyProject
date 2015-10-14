/*Auto generated code*/

#ifndef INTERRUPT_VECTOR_H_
#define INTERRUPT_VECTOR_H_

#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

extern "C" void USARTC0_RXC_vect(void) __attribute__ ((signal));
extern "C" void USARTC0_DRE_vect(void) __attribute__ ((signal));
extern "C" void USARTC0_TXC_vect(void) __attribute__ ((signal));
extern "C" void USARTC1_RXC_vect(void) __attribute__ ((signal));
extern "C" void USARTC1_DRE_vect(void) __attribute__ ((signal));
extern "C" void USARTC1_TXC_vect(void) __attribute__ ((signal));
extern "C" void USARTD0_RXC_vect(void) __attribute__ ((signal));
extern "C" void USARTD0_DRE_vect(void) __attribute__ ((signal));
extern "C" void USARTD0_TXC_vect(void) __attribute__ ((signal));
extern "C" void USARTD1_RXC_vect(void) __attribute__ ((signal));
extern "C" void USARTD1_DRE_vect(void) __attribute__ ((signal));
extern "C" void USARTD1_TXC_vect(void) __attribute__ ((signal));
extern "C" void USARTE0_RXC_vect(void) __attribute__ ((signal));
extern "C" void USARTE0_DRE_vect(void) __attribute__ ((signal));
extern "C" void USARTE0_TXC_vect(void) __attribute__ ((signal));
extern "C" void USARTE1_RXC_vect(void) __attribute__ ((signal));
extern "C" void USARTE1_DRE_vect(void) __attribute__ ((signal));
extern "C" void USARTE1_TXC_vect(void) __attribute__ ((signal));
extern "C" void USARTF0_RXC_vect(void) __attribute__ ((signal));
extern "C" void USARTF0_DRE_vect(void) __attribute__ ((signal));
extern "C" void USARTF0_TXC_vect(void) __attribute__ ((signal));
extern "C" void USARTF1_RXC_vect(void) __attribute__ ((signal));
extern "C" void USARTF1_DRE_vect(void) __attribute__ ((signal));
extern "C" void USARTF1_TXC_vect(void) __attribute__ ((signal));

struct UsartInterrupt {
	class ReceiveListenner {
	public:
		virtual void onUsartReceive() = 0;
	};
	ReceiveListenner * Receive;
	
	class DataRegisterEmptyListenner {
	public:
		virtual void onUsartDataRegisterEmpty() = 0;
	};
	DataRegisterEmptyListenner * DataRegisterEmpty;
	
	class TransmitListenner {
	public:
		virtual void onUsartTransmit() = 0;
	};
	TransmitListenner * Transmit;
	
};

extern "C" void TCC0_OVF_vect(void) __attribute__ ((signal));
extern "C" void TCC0_ERR_vect(void) __attribute__ ((signal));
extern "C" void TCC0_CCA_vect(void) __attribute__ ((signal));
extern "C" void TCC0_CCB_vect(void) __attribute__ ((signal));
extern "C" void TCC0_CCC_vect(void) __attribute__ ((signal));
extern "C" void TCC0_CCD_vect(void) __attribute__ ((signal));
extern "C" void TCD0_OVF_vect(void) __attribute__ ((signal));
extern "C" void TCD0_ERR_vect(void) __attribute__ ((signal));
extern "C" void TCD0_CCA_vect(void) __attribute__ ((signal));
extern "C" void TCD0_CCB_vect(void) __attribute__ ((signal));
extern "C" void TCD0_CCC_vect(void) __attribute__ ((signal));
extern "C" void TCD0_CCD_vect(void) __attribute__ ((signal));
extern "C" void TCE0_OVF_vect(void) __attribute__ ((signal));
extern "C" void TCE0_ERR_vect(void) __attribute__ ((signal));
extern "C" void TCE0_CCA_vect(void) __attribute__ ((signal));
extern "C" void TCE0_CCB_vect(void) __attribute__ ((signal));
extern "C" void TCE0_CCC_vect(void) __attribute__ ((signal));
extern "C" void TCE0_CCD_vect(void) __attribute__ ((signal));
extern "C" void TCF0_OVF_vect(void) __attribute__ ((signal));
extern "C" void TCF0_ERR_vect(void) __attribute__ ((signal));
extern "C" void TCF0_CCA_vect(void) __attribute__ ((signal));
extern "C" void TCF0_CCB_vect(void) __attribute__ ((signal));
extern "C" void TCF0_CCC_vect(void) __attribute__ ((signal));
extern "C" void TCF0_CCD_vect(void) __attribute__ ((signal));
extern "C" void TCC1_OVF_vect(void) __attribute__ ((signal));
extern "C" void TCC1_ERR_vect(void) __attribute__ ((signal));
extern "C" void TCC1_CCA_vect(void) __attribute__ ((signal));
extern "C" void TCC1_CCB_vect(void) __attribute__ ((signal));
extern "C" void TCC1_CCC_vect(void) __attribute__ ((signal));
extern "C" void TCC1_CCD_vect(void) __attribute__ ((signal));
extern "C" void TCD1_OVF_vect(void) __attribute__ ((signal));
extern "C" void TCD1_ERR_vect(void) __attribute__ ((signal));
extern "C" void TCD1_CCA_vect(void) __attribute__ ((signal));
extern "C" void TCD1_CCB_vect(void) __attribute__ ((signal));
extern "C" void TCD1_CCC_vect(void) __attribute__ ((signal));
extern "C" void TCD1_CCD_vect(void) __attribute__ ((signal));
extern "C" void TCE1_OVF_vect(void) __attribute__ ((signal));
extern "C" void TCE1_ERR_vect(void) __attribute__ ((signal));
extern "C" void TCE1_CCA_vect(void) __attribute__ ((signal));
extern "C" void TCE1_CCB_vect(void) __attribute__ ((signal));
extern "C" void TCE1_CCC_vect(void) __attribute__ ((signal));
extern "C" void TCE1_CCD_vect(void) __attribute__ ((signal));
extern "C" void TCF1_OVF_vect(void) __attribute__ ((signal));
extern "C" void TCF1_ERR_vect(void) __attribute__ ((signal));
extern "C" void TCF1_CCA_vect(void) __attribute__ ((signal));
extern "C" void TCF1_CCB_vect(void) __attribute__ ((signal));
extern "C" void TCF1_CCC_vect(void) __attribute__ ((signal));
extern "C" void TCF1_CCD_vect(void) __attribute__ ((signal));

struct CounterInterrupt {
	class OverflowListenner {
	public:
		virtual void onCounterOverflow() = 0;
	};
	OverflowListenner * Overflow;
	
	class ErrorListenner {
	public:
		virtual void onCounterError() = 0;
	};
	ErrorListenner * Error;
	
	class ChannelListenner {
	public:
		virtual void onCounterChannel() = 0;
	};
	ChannelListenner * Channel[4];
	
};

extern "C" void TWIC_TWIS_vect(void) __attribute__ ((signal));
extern "C" void TWIC_TWIM_vect(void) __attribute__ ((signal));
extern "C" void TWID_TWIS_vect(void) __attribute__ ((signal));
extern "C" void TWID_TWIM_vect(void) __attribute__ ((signal));
extern "C" void TWIE_TWIS_vect(void) __attribute__ ((signal));
extern "C" void TWIE_TWIM_vect(void) __attribute__ ((signal));
extern "C" void TWIF_TWIS_vect(void) __attribute__ ((signal));
extern "C" void TWIF_TWIM_vect(void) __attribute__ ((signal));

struct TwiInterrupt {
	class SlaveEventListenner {
	public:
		virtual void onTwiSlaveEvent() = 0;
	};
	SlaveEventListenner * SlaveEvent;
	
	class MasterEventListenner {
	public:
		virtual void onTwiMasterEvent() = 0;
	};
	MasterEventListenner * MasterEvent;
	
};


struct PortInterrupt {
};

#endif /* INTERRUPT_VECTOR_H_ */
