#include "Devices/xbee.h"

XBee::XBee(QScopedPointer<QSerialPort>& port)
{
    connect( &timer_, SIGNAL( timeout() ), this, SLOT( handle() ) );
    timer_.start( 100 );
    port_.swap(port);

    bool status = port_->open(QIODevice::ReadWrite);
    if (status == true) {
        std::cout << "xbee connected" << std::endl;
    }
}

XBee::~XBee()
{
}

void XBee::read(XBee::ResponsePacket* packet)
{
    if (packet->isAtEnd()) {
        packet->reset();
    }
    while (!packet->isAtEnd() && port_->bytesAvailable()) {
        char byte;
        port_->getChar(&byte);
        packet->write(byte);
    }
}

void XBee::write(XBee::RequestPacket packet)
{
    while (!packet.isAtEnd()) {
        uint8_t bou = packet.read();
        port_->putChar(bou);
        //std::cout << int(bou) << " ";
    }
    //std::cout << std::endl;
}

void XBee::handle()
{
    if (port_->error() != QSerialPort::NoError) {
        port_->close();
        std::cout << "xbee diconnected : " << port_->error() << std::endl;
        QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
        for (auto portIt = ports.begin(); portIt != ports.end(); portIt++) {
            /*std::cout << int(portIt->productIdentifier()) << std::endl;
            std::cout << int(portIt->vendorIdentifier()) << std::endl;
            std::cout << portIt->serialNumber().toStdString() << std::endl;
            std::cout << portIt->systemLocation().toStdString() << std::endl;
            std::cout << portIt->manufacturer().toStdString() << std::endl << std::endl;*/
            if (portIt->productIdentifier() == 24577 && portIt->vendorIdentifier() == 1027) {
                port_->setPort(*portIt);
            }
        }
        bool status = port_->open(QIODevice::ReadWrite);
        if (status == true) {
            std::cout << "xbee connected" << std::endl;
        }
    }
}


