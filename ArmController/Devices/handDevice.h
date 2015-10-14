#ifndef HAND_DEVICE_H
#define HAND_DEVICE_H

#include "State/ArmKnowledge.h"
#include "xbee.h"
#include <iostream>


class HandDevice : public QObject {
    Q_OBJECT
public:
    HandDevice() = default;
    HandDevice(XBee* xbee);
    ~HandDevice();

    void sendInstructions(HandInstructions instructions);

signals:
    void observationsReceive(ImuObservationsBare observations);

public slots:
    void onReadyRead();

private:
    HandInstructions instructions_;
    ImuObservationsBare observations_;
    XBee::ResponsePacket responsePacket_;

    XBee* xbee_;

    QMetaObject::Connection xbeeReception_;
};


#endif // HAND_DEVICE_H
