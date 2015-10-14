/*
 * TWI.h
 *
 * Created: 2014-07-16 20:58:14
 *  Author: Etienne
 */ 


#ifndef TWI_H_
#define TWI_H_

#include "../Controller/ModuleController.h"
#include "../DataStructure/Buffer.h"
#include "../DataStructure/Queue.h"
#include "../DataStructure/UniquePtr.h"


const uint8_t TOKEN_ADDRESS = 0xff;
enum RequestStatus : uint8_t {REQUEST_PENDING, REQUEST_DONE, REQUEST_FAILED};

class TwiMaster : public TwiInterrupt::MasterEventListenner {
public:
    TwiMaster(UniquePtr<TwiController>&& twiController, uint32_t frequency);
	~TwiMaster();
	void reset();
    
    void write(uint8_t address, const Buffer<uint8_t>& buffer);
    void write(uint8_t address, const uint8_t data[], uint8_t lenght);
    void read(uint8_t address, Buffer<uint8_t>& buffer);
    void read(uint8_t address, uint8_t data[], uint8_t lenght);
    void insertToken(volatile RequestStatus* status);
    uint8_t getState() const;
    
protected:
	void onTwiMasterEvent();
	
private:
	TwiMaster();
	TwiMaster(const TwiMaster&);
	TwiMaster& operator = (const TwiMaster&);
    
    class Packet {
    public:
        Packet() {index_ = 0; address_ = 0;}
        Packet(uint8_t address, Buffer<uint8_t> buffer);
        Packet(volatile RequestStatus* status);
        
        bool isToken() const;
        void setToken(volatile RequestStatus state) {buffer_.write(state, 0);}
        uint8_t getAddress() const {return address_;}
        uint8_t getIndex() const {return index_;}
        uint8_t pull() {return buffer_.read(index_);}
        void pop() {index_+=1;}
        void push(uint8_t data);
        
        Buffer<uint8_t> getData() {return buffer_;}
        
        bool isAtEnd() {return index_ == buffer_.lenght();}
        void setToEnd() {index_ = buffer_.lenght();}
        
    private:
        volatile uint8_t address_;
		Buffer<uint8_t> buffer_;
        volatile uint8_t index_;
    };
    
    void push(Packet packet);
	
	UniquePtr<TwiController> twiController_;
    volatile Queue<Packet> queue_;
    volatile RequestStatus status_;
	
	TWI_t* twi_;
};

#endif /* TWI_H_ */