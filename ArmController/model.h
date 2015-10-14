#ifndef MODEL_H
#define MODEL_H

#include <QQuaternion>

#include "Leap.h"

#include "State/Imu.h"
#include "State/ArmKnowledge.h"
#include "State/Matrix.h"

class Model
{
public:
    Model();
    ~Model();

    void updateLeap(const Leap::Controller&);
    void updateHand(ImuObservationsBare observations);

    HandObjective getObjective() const;
    HandInstructions getInstructions();

    HandBasis getBasis() const {return handBasis_;}
    QQuaternion getImu() const;

private:
    static double angle(Leap::Matrix a, Leap::Matrix b, Leap::Vector normal);

    Imu imu_;
    HandInstructions instructions_;
    HandObjective objective_;
    HandBasis handBasis_;
    HandParameter parameter_;
};

#endif // MODEL_H
