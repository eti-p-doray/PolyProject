#include "Controller.h"


Controller::Controller()
{
    glView_ = new GlView();
    glView_->show();
    glView_->setModel(&model_);

    QScopedPointer<QSerialPort> port(new QSerialPort()); //"/dev/tty.usbserial-A900UDWZ"
    port->setBaudRate(57600);
    port->setDataBits(QSerialPort::Data8);
    port->setFlowControl(QSerialPort::NoFlowControl);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);

    xbee_.reset(new XBee(port));
    hand_.reset(new HandDevice(xbee_.data()));

    leapController_.addListener(*this);
    armReception_ = connect(hand_.data(), &HandDevice::observationsReceive, this, &Controller::onObservationReceive);

    connect( &timer_, SIGNAL( timeout() ), this, SLOT( refreshView() ) );
    timer_.start( 60 );
    elapsedtimer_.start();
}

Controller::~Controller()
{
    leapController_.removeListener(*this);
    QObject::disconnect(armReception_);
    delete glView_;
}

void Controller::onObservationReceive(ImuObservationsBare observations)
{
    interval.set(elapsedtimer_.nsecsElapsed());
    //std::cout << interval.getAverage() << std::endl;
    //std::cout << elapsedtimer_.nsecsElapsed() << std::endl;
    elapsedtimer_.restart();
    ImuObservations<double> ob(observations);

    ImuParameter<double> param = ImuParameter<double>::getInitial();

    /*data[0].set((ob.accl(0,0)-param.translAccl(0,0))/param.scaleAccl(0,0));
    data[1].set((ob.accl(1,0)-param.translAccl(0,0))/param.scaleAccl(1,0));
    data[2].set((ob.accl(2,0)-param.translAccl(0,0))/param.scaleAccl(2,0));
    data[3].set((ob.magn(0,0)-param.translMagn(0,0))/param.scaleMagn(0,0));
    data[4].set((ob.magn(1,0)-param.translMagn(1,0))/param.scaleMagn(1,0));
    data[5].set((ob.magn(2,0)-param.translMagn(2,0))/param.scaleMagn(2,0));*/

    /*data[0].set(ob.magn(0,0));
    data[1].set(ob.magn(1,0));
    data[2].set(ob.magn(2,0));*/

    /*std::cout << std::setw(10) << std::left << ob.accl(0,0)
                << std::setw(10) << std::left << ob.accl(1,0)
                << std::setw(10) << std::left << ob.accl(2,0)
                << std::setw(10) << std::left << ob.gyro(0,0)
                << std::setw(10) << std::left << ob.gyro(1,0)
                << std::setw(10) << std::left << ob.gyro(2,0)
                << std::endl;*/

    /*std::cout << std::setw(15) << std::left << ob.magn(0,0)
               << std::setw(15) << std::left << ob.magn(1,0)
              << std::setw(15) << std::left  << ob.magn(2,0)
              << std::setw(15) << std::left  << data[3].getVariance()
               << std::setw(15) << std::left << data[4].getVariance()
               << std::setw(15) << std::left << data[5].getVariance() << std::endl;*/

    /*std::cout << std::setw(10) << std::left << data[0].getAverage()
              << std::setw(10) << std::left << data[1].getAverage()
              << std::setw(10) << std::left << data[2].getAverage()
              << std::setw(10) << std::left << data[3].getAverage()
              << std::setw(10) << std::left << data[4].getAverage()
              << std::setw(10) << std::left << data[5].getAverage() << std::endl;*/

    model_.updateHand(observations);
    hand_->sendInstructions(model_.getInstructions());
    //glView_->update();
}

void Controller::refreshView()
{
    //std::cout << model_.getInstructions().fingers[2].joints[1].position << std::endl;
    //hand_->sendInstructions(model_.getInstructions());
    glView_->update();
}

void Controller::onConnect(const Leap::Controller& controller) {
    std::cout << "Leap connected" << std::endl;
}

void Controller::onDisconnect(const Leap::Controller& controller) {
    std::cout << "Leap disconnected" << std::endl;
}

void Controller::onFrame(const Leap::Controller & controller) {
    model_.updateLeap(controller);
}
