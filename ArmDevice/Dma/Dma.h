/*
 * MemoryController.h
 *
 * Created: 2014-09-14 20:59:49
 *  Author: Etienne
 */ 


#ifndef MEMORYCONTROLLER_H_
#define MEMORYCONTROLLER_H_

//enum class EDMAStandardChannelNumber {channel0, channel2, Count};
//enum class EDMAPeripheralChannelNumber {channel0, channel1, channel2, channel3, Count};

class Dma {
public:
    class Channel {
        
    }
    
	Dma();
    MemoryController(DmaController::Index* index);
	~MemoryController();
	
	void enable();
	void disable();
	
	MemoryStandardChannel* createStandardChannel(EDMAChannelNumber channel);
	MemoryPeripheralChannel* createPeripheralChannel(EDMAChannelNumber channel);
	
private:
	UniquePtr<DmaController> dmaController_;
    UniquePtr<Channel> channels_[static_cast<uint8_t>(DmaController::Channel::Index::Count)];
};


#endif /* MEMORYCONTROLLER_H_ */