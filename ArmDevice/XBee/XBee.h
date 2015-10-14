/*
 * SERIAL.h
 *
 * Created: 2014-08-05 21:15:33
 *  Author: Etienne
 */ 


#ifndef SERIAL_H_
#define SERIAL_H_

#include "../Usart/Usart.h"
#include "../Controller/ModuleController.h"
#include "../DataStructure/Queue.h"
#include "../DataStructure/Buffer.h"

const uint8_t START_DELIMITER = 0x7E;

class XBee {
public:
    enum class Type : uint8_t {INVALID = 0xff, TRANSMIT64_REQUEST = 0x00, TRANSMIT16_REQUEST = 0x01, TRANSMIT_STATUS = 0x89,
		ZB_TRANSMIT_REQUEST = 0x10, ZB_TRANSMIT_STATUS = 0x8B, RECEIVE64 = 0x80, RECEIVE16 = 0x81, ZB_RECEIVE = 0x90};
    class RequestPacket;
    class ResponsePacket;
    
    class RequestFrameData {
        friend class XBee::RequestPacket;
    public:
        virtual ~RequestFrameData() {}
        virtual Type getType() const = 0;
        virtual uint16_t lenght() const = 0;
        
    protected:
        virtual uint8_t read(uint16_t index) const = 0;
    };
    
    class ResponseFrameData {
        friend class XBee::ResponsePacket;
    public:
        virtual ~ResponseFrameData() {}
        virtual Type getType() const = 0;
        virtual uint16_t lenght() const = 0;
    protected:
        virtual void write(uint8_t data, uint16_t index) = 0;
    };
    
    class InvalidFrameData : public RequestFrameData, public ResponseFrameData {
    public:
        ~InvalidFrameData() {}
        
        Type getType() const {return Type::INVALID;}
        uint16_t lenght() const {return 0;}
    protected:
        uint8_t read(uint16_t index) const {return 0;}
        void write(uint8_t data, uint16_t index) {}
    };
	
	class Transmit64Request : public RequestFrameData {
	public:
		Transmit64Request(uint64_t destinationAddress, Buffer<uint8_t> buffer);
		Transmit64Request(uint64_t destinationAddress, uint8_t* data, uint16_t lenght);
		
		Type getType() const {return Type::TRANSMIT64_REQUEST;}
		uint16_t lenght() const {return buffer_.lenght() + 11;}
			
		void setId(uint8_t id) {id_ = id;}
		void setDestinationAddress(uint64_t address) {destinationAddress_ = address;}
		void setOptions(uint8_t options) {options_ = options;}
		void setBuffer(Buffer<uint8_t> buffer) {buffer_ = buffer;}

	protected:
		uint8_t read(uint16_t index) const;
		
	private:
		uint8_t id_;
		uint64_t destinationAddress_;
		uint8_t options_;
		Buffer<uint8_t> buffer_;	
	};
	
	class Transmit16Request : public RequestFrameData {
	public:
		Transmit16Request(uint16_t destinationAddress, Buffer<uint8_t> buffer);
		Transmit16Request(uint16_t destinationAddress, uint8_t* data, uint16_t lenght);
		
		Type getType() const {return Type::TRANSMIT16_REQUEST;}
		uint16_t lenght() const {return buffer_.lenght() + 5;}
		
		void setId(uint8_t id) {id_ = id;}
		void setDestinationAddress(uint16_t address) {destinationAddress_ = address;}
		void setOptions(uint8_t options) {options_ = options;}
		void setBuffer(Buffer<uint8_t> buffer) {buffer_ = buffer;}

	protected:
		uint8_t read(uint16_t index) const;
		
	private:
		uint8_t id_;
		uint16_t destinationAddress_;
		uint8_t options_;
		Buffer<uint8_t> buffer_;
	};
	
	class TransmitStatus : public ResponseFrameData {
	public:
		TransmitStatus();
		
		Type getType() const {return Type::TRANSMIT_STATUS;}
		uint16_t lenght() const {return 3;}
		
		uint8_t getId() const {return id_;}
		uint8_t getStatus() const {return status_;}
		
	protected:
		void write(uint8_t data, uint16_t index);
	private:
		uint8_t id_;
		uint8_t status_;
	};
    
    class ZbTransmitRequest : public RequestFrameData {
    public:
        ZbTransmitRequest(uint64_t destinationAddress, uint16_t destinationNetwork, Buffer<uint8_t> buffer);
        ZbTransmitRequest(uint64_t destinationAddress, uint16_t destinationNetwork, uint8_t* data, uint16_t lenght);
        
        Type getType() const {return Type::ZB_TRANSMIT_REQUEST;}
        uint16_t lenght() const {return buffer_.lenght() + 14;}
        
        void setId(uint8_t id) {id_ = id;}
        void setDestinationAddress(uint64_t address) {destinationAddress_ = address;}
        void setDestinationNetwork(uint16_t network) {destinationNetwork_ = network;}
        void setBroadcastRadius(uint8_t radius) {broadcastRadius_ = radius;}
        void setOptions(uint8_t options) {options_ = options;}
        void setBuffer(Buffer<uint8_t> buffer) {buffer_ = buffer;}
    protected:
        uint8_t read(uint16_t index) const;
        
    private:
        uint8_t id_;
        uint64_t destinationAddress_;
        uint16_t destinationNetwork_;
        uint8_t broadcastRadius_;
        uint8_t options_;
        Buffer<uint8_t> buffer_;
    };
    
    class ZbTransmitStatus : public ResponseFrameData {
    public:
        ZbTransmitStatus();
        
        Type getType() const {return Type::ZB_TRANSMIT_STATUS;}
        uint16_t lenght() const {return 6;}
        
        uint8_t getId() const {return id_;}
        uint16_t getDestinationNetwork() const {return destinationNetwork_;}
        uint8_t getRetryCount() const {return retryCount_;}
        uint8_t getDeliveryStatus() const {return deliveryStatus_;}
        uint8_t getDiscoveryStatus() const {return discoveryStatus_;}
        
    protected:
        void write(uint8_t data, uint16_t index);
    private:
        uint8_t id_;
        uint16_t destinationNetwork_;
        uint8_t retryCount_;
        uint8_t deliveryStatus_;
        uint8_t discoveryStatus_;
    };
    
	class Receive64 : public ResponseFrameData {
	public:
		Receive64(Buffer<uint8_t> buffer);
		
		Type getType() const {return Type::RECEIVE64;}
		uint16_t lenght() const {return buffer_.lenght()+11;}
		
		uint64_t getSourceAddress() const {return sourceAddress_;}
		uint8_t getRssi() const {return rssi_;}
		uint8_t getOptions() const {return options_;}
		Buffer<uint8_t> getData() const {return buffer_;}
			
		template <typename T> T getData() {return *(T*)getData().read();}
		
	protected:
		void write(uint8_t data, uint16_t index);
		
	private:
		uint64_t sourceAddress_;
		uint8_t rssi_;
		uint8_t options_;
		Buffer<uint8_t> buffer_;
	};
	
	class Receive16 : public ResponseFrameData {
	public:
		Receive16(Buffer<uint8_t> buffer);
		
		Type getType() const {return Type::RECEIVE16;}
		uint16_t lenght() const {return buffer_.lenght()+5;}
		
		uint16_t getSourceAddress() const {return sourceAddress_;}
		uint8_t getRssi() const {return rssi_;}
		uint8_t getOptions() const {return options_;}
		Buffer<uint8_t> getData() const {return buffer_;}
			
		template <typename T> T getData() {return *(T*)getData().read();}
			
	protected:
		void write(uint8_t data, uint16_t index);
		
	private:
		uint16_t sourceAddress_;
		uint8_t rssi_;
		uint8_t options_;
		Buffer<uint8_t> buffer_;
	};
	
    class ZbReceive : public ResponseFrameData {
    public:
        ZbReceive(Buffer<uint8_t> buffer);
        
        Type getType() const {return Type::ZB_RECEIVE;}
        uint16_t lenght() const {return buffer_.lenght()+12;}
        
        uint64_t getSourceAddress() const {return sourceAddress_;}
        uint16_t getSourceNetwork() const {return sourceNetwork_;}
        uint8_t getOptions() const {return options_;}
        Buffer<uint8_t> getData() const {return buffer_;}
		
		template <typename T> T getData() {return *(T*)getData().read();}	
			
    protected:
        void write(uint8_t data, uint16_t index);
        
    private:
        uint64_t sourceAddress_;
        uint16_t sourceNetwork_;
        uint8_t options_;
        Buffer<uint8_t> buffer_;
    };
    
    class Packet {
    public:
        Packet();
        bool isAtEnd() const;
		virtual void reset();
        
    protected:
        uint8_t checksum_;
        int16_t index_;
        int16_t lenght_;
    };
    
    class RequestPacket : public Packet {
    public:
        RequestPacket(RequestFrameData* frameData);
        uint8_t read();
        
    private:
        const RequestFrameData* frameData_;
    };
    
    class ResponsePacket : public Packet {
    public:
        ResponsePacket(uint16_t bufferLenght = 255);
		bool isValid() const;
		Type getFrameType() {return frameData_->getType();}
        UniquePtr<ResponseFrameData> getFrame();
        void write(uint8_t data);
		
		template <typename T> T getFrame()
		{
			T ans = *(T*)frameData_.getPtr();
			frameData_ = new InvalidFrameData;
			return ans;
		}
        
    private:
        UniquePtr<ResponseFrameData> frameData_;
        UniqueBuffer<uint8_t> buffer_;
    };
    
	XBee(UniquePtr<UsartController>&& usartController);
	~XBee();
    
    void read(ResponsePacket* packet);
    void write(RequestPacket packet);
	
	void map(PinLocation location);
	
private:
    Usart usart_;
};

#endif /* SERIAL_H_ */