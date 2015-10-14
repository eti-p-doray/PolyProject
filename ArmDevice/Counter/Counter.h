/*
 * Counter.h
 *
 * Created: 2014-09-02 09:28:19
 *  Author: Etienne
 */ 


#ifndef COUNTER_H_
#define COUNTER_H_

#include "../Controller/ModuleController.h"

class Counter {
public:
    class Channel {
        friend class Counter;
    public:
        enum class Index {A, B, C, D, COUNT};
        
        ~Channel();
        
        void write(uint16_t value);
        uint16_t read() const;
        Index getIndex() const {return channelIndex_;}
        uint16_t getPeriode() const {return counterController_->module()->PER;}
        
        void enablePin(PinLocation location);
        void disablePin();
        
        void map(PinLocation location);
        
        void set() {counterController_->module()->CTRLC |= TC0_CMPA_bm << static_cast<uint8_t>(channelIndex_);}
        void clear() {counterController_->module()->CTRLC &= ~(TC0_CMPA_bm << static_cast<uint8_t>(channelIndex_));}
        
        void listen(CounterInterrupt::ChannelListenner& manager, TC_CCAINTLVL_enum interruptLevel);
        
    private:
        Channel(CounterController* counterController, Index channelIndex);
        Channel(CounterController* counterController, PinLocation location, Index channelIndex);
        Channel(Channel&);
        Channel& operator=(Channel&);
        
        CounterController* counterController_;
        Index channelIndex_;
		
		PORT_t* port;
		TC0_t* tc;
    };
    
    Counter(UniquePtr<CounterController>&& counterController, TC_CLKSEL_enum source, uint16_t periode, TC_WGMODE_enum mode);
	~Counter();
    
	uint16_t read() const;
	void write(uint16_t value);
    uint16_t getPeriode() const;
    
    void selectEventSource(uint8_t eventChannel);
    void selectEventAction(TC_EVACT_enum action);
    
	void listenOverflow(CounterInterrupt::OverflowListenner* listenner, TC_OVFINTLVL_enum interruptLevel);
    void listenError(CounterInterrupt::ErrorListenner* listenner, TC_ERRINTLVL_enum interruptLevel);
    
    void configure(TC_CLKSEL_enum source, uint16_t periode, TC_WGMODE_enum mode);
    void configure(TC_CLKSEL_enum source);
    
    Channel* createChannel(Channel::Index index);
    Channel* createChannel(PinLocation location, Channel::Index index);
	
	bool isOverflow() {return (counterController_->module()->INTFLAGS & TC0_OVFIF_bm) != 0;}
	void clearOverflow() {counterController_->module()->INTFLAGS |= TC0_OVFIF_bm;}
	
	
private:
    UniquePtr<CounterController> counterController_;
    UniquePtr<Channel> channels_[static_cast<uint8_t>(Channel::Index::COUNT)];
	
	TC0_t* tc;
};


#endif /* COUNTER_H_ */