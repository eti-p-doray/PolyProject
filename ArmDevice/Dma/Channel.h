/*
 * Channel.h
 *
 * Created: 2014-09-14 21:04:05
 *  Author: Etienne
 */ 


#ifndef CHANNEL_H_
#define CHANNEL_H_

MemoryChannel {
public:
	void enable();
	void disable();	
	void reset();
	void enableRepeat();
	void disableRepeat();
	void requestTransfert();
	void enableSingleBurst();
	void disableSingleBurst();
	void setBurstLenght1();
	void setBurstLenght2();
	void setDataMatch(EDMA_CH_DIR_enum mode, uint8_t lsb, uint8_t msb);
	void setTrigger(EDMA_CH_TRIGSRC_enum);
	
	void listen(EDMAChannelInterrupt::EDMAChannelManager* manager, EDMA_CH_INTLVL_enum interruptLevel);
    
private:
	EDMAChannel* module;
};


class MemoryStandardChannel : public MemoryChannel {
	friend class MemoryController;
public:
	void setReload(EDMA_CH_RELOAD_enum);
	void setAddressMode(EDMA_CH_DIR_enum mode);
	void setBlockSize(uint8_t newSize);
	void setAddress(void* address);

private:
	MemoryStandardChannel();

};

class MemoryPeripheralChannel :public MemoryChannel {
	friend class MemoryController;
public:
	void setSourceReload(EDMA_CH_SRC);
	void setSourceAddressMode(EDMA_CH_DIR_enum mode);
	void setDestinationReload(EDMA_CH_SRC);
	void setDestinationAddressMode(EDMA_CH_DIR_enum mode);
	void setBlockSize(uint16_t newSize);
	void setSourceAddress(void* address);
	void setDestinationAddress(void* address);

private:
	MemoryPeripheralChannel();
	
	EDMAChannel* module;
};


#endif /* CHANNEL_H_ */