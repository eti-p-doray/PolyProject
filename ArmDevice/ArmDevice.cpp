/*
 * ArmDevice.cpp
 *
 * Created: 2015-02-04 07:52:47
 *  Author: Etienne
 */ 

#include "util/delay.h"
#include "Xbee/XBee.h"
#include "Imu.h"
#include "Timer/Timer.h"
#include "ArmKnowledge.h"
#include "Hand.h"

void configureClock() {
    OSC.XOSCCTRL = OSC_FRQRANGE_12TO16_gc | OSC_XOSCSEL_XTAL_16KCLK_gc;
	OSC.CTRL = OSC_XOSCEN_bm;
	while(!(OSC.STATUS & OSC_XOSCRDY_bm));
	OSC.PLLCTRL = OSC_PLLSRC_XOSC_gc | 0x04;
	OSC.CTRL |= OSC_PLLEN_bm;
	while(!(OSC.STATUS & OSC_PLLRDY_bm));
	
	CCP = CCP_IOREG_gc;
	CLK.PSCTRL = CLK_PSADIV_1_gc | CLK_PSBCDIV_1_1_gc;
	CCP = CCP_IOREG_gc;
	CLK.CTRL = CLK_SCLKSEL_PLL_gc;
}

int main(void)
{
    configureClock();
    
    XBee xbee(UsartController::instance(UsartController::Index::USARTF1_IDX));
	//xbee.map(PinLocation::REMAP);
	
    Imu imu;
    Timer timer(CounterController::instance(CounterController::Index::TCF1_IDX), 10);
	
    Hand hand;
	ServoController controller(CounterController::instance(CounterController::Index::TCF1_IDX), 50);
	Servo* servo = controller.createServo(Counter::Channel::Index::C, PinLocation::NORMAL);
	servo->setRange(900, 2100);
    
    ImuObservationsBare observations;
    HandInstructions instructions;
    XBee::ResponsePacket responsePacket(sizeof(HandInstructions));
	
	//hand.write(instructions);
	//servo->write(0x8000);
		
    while(true)
    {
		xbee.read(&responsePacket);
		if (responsePacket.isValid()) {
			if (responsePacket.getFrameType() == XBee::Type::RECEIVE16) {
				instructions = responsePacket.getFrame<XBee::Receive16>().getData<HandInstructions>();
				//servo->write(instructions.forearm.wristFlexion[0].position);
				hand.write(instructions);
			}
			/*else {
				volatile int bou = 0;
			}*/
		}
        if (timer.isReady()) {

            bool status = imu.read(observations);
			if (status == true) {
				timer.clear();
			}
        }
        if (imu.isReady()) {
			imu.clear();
			//XBee::ZbTransmitRequest request(0x0013a200408cd2c5, 0xfffe, (uint8_t*)&observations, sizeof(observations));
			XBee::Transmit16Request request(0x01, (uint8_t*)&observations, sizeof(observations));
			//request.setId(01);
			//XBee::Transmit64Request request(0x0013a20040717A9A, (uint8_t*)&observations, sizeof(observations));
			xbee.write(&request);
        }
    }
}