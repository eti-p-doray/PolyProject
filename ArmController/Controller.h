#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include <iomanip>
#include "Leap.h"

#include "model.h"
#include "State/ArmKnowledge.h"
#include "Devices/handDevice.h"
#include "Average.h"
#include "GlView.h"


class Controller : public QObject, public Leap::Listener {
    Q_OBJECT
public:
    Controller();
    ~Controller();

    virtual void onConnect(const Leap::Controller&);
    virtual void onDisconnect(const Leap::Controller&);
    virtual void onFrame(const Leap::Controller&);
    
private slots:
    void onObservationReceive(ImuObservationsBare observations);
    void refreshView();

private:
    GlView * glView_;
    Model model_;

    QScopedPointer<HandDevice> hand_;
    QScopedPointer<XBee> xbee_;
    Leap::Controller leapController_;

    QMetaObject::Connection armReception_;

    QTimer timer_;
    QElapsedTimer elapsedtimer_;
    Signal interval;

    Signal data[6];
};

#endif
