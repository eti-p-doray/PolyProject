#include "model.h"
#include <iomanip>

Model::Model()
{
    objective_ = {};
    instructions_ = {};
}

Model::~Model()
{

}

void Model::updateLeap(const Leap::Controller & controller) {
    Leap::Frame frame = controller.frame();
    if (frame.isValid() != true) {
        return;
    }
    Leap::HandList hands = frame.hands();

    Leap::Hand hand = Leap::Hand::invalid();
    for(int i = 0; i < hands.count(); i++) {
        if ( hands[i].isLeft() ) {
            hand = hands[i];
            break;
        }
    }

    if (!hand.isValid()) {
        return;
    }

    objective_.forearm.wristFlexion[0].position = angle(hand.basis(), hand.arm().basis(), hand.arm().basis().xBasis);
    objective_.forearm.wristFlexion[1].position = angle(hand.basis(), hand.arm().basis(), hand.arm().basis().yBasis);
    //objective.hand.wristRotation = angle(Leap::Matrix().identity(), hand.arm().basis(), hand.arm().basis().zBasis);
    //objective.hand.elbowFlexion = angle(Leap::Matrix().identity(), hand.arm().basis(), hand.arm().basis().xBasis);


    Leap::FingerList fingers = hand.fingers();

    for (int i = 0; i < fingers.count(); i++) {
        objective_.fingers[i].joints[0].position = angle(fingers[i].bone(Leap::Bone::Type(1)).basis(), fingers[i].bone(Leap::Bone::Type(0)).basis(), fingers[i].bone(Leap::Bone::Type(0)).basis().yBasis);
        for (int j = 1; j < 4; j++) {
            objective_.fingers[i].joints[j].position = angle(fingers[i].bone(Leap::Bone::Type(j)).basis(), fingers[i].bone(Leap::Bone::Type(j-1)).basis(), fingers[i].bone(Leap::Bone::Type(j-1)).basis().xBasis);
        }
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 4; j++) {
            handBasis_.fingers[i].joints[j].orientation = fingers[i].bone(Leap::Bone::Type(j)).basis().toMatrix4x4<QMatrix4x4>();
            //handBasis_.fingers[i].joints[j].orientation.column(2) *= -1;
            handBasis_.fingers[i].joints[j].position = fingers[i].bone(Leap::Bone::Type(j)).center().toVector3<QVector3D>();
            handBasis_.fingers[i].joints[j].scale.setX(0.5*fingers[i].bone(Leap::Bone::Type(j)).width());
            handBasis_.fingers[i].joints[j].scale.setY(0.5*fingers[i].bone(Leap::Bone::Type(j)).width());
            handBasis_.fingers[i].joints[j].scale.setZ(0.5*fingers[i].bone(Leap::Bone::Type(j)).length());
        }
    }
    handBasis_.palm.orientation = hand.basis().toMatrix4x4<QMatrix4x4>();
    //handBasis_.palm.orientation.column(2) *= -1;
    handBasis_.palm.position = hand.palmPosition().toVector3<QVector3D>();
    handBasis_.palm.scale.setX(0.5*hand.palmWidth());
    handBasis_.palm.scale.setY(10);
    handBasis_.palm.scale.setZ(0.5*hand.palmWidth());
    handBasis_.forearm.orientation = hand.arm().basis().toMatrix4x4<QMatrix4x4>();
    //handBasis_.forearm.orientation.column(2) *= -1;
}

void Model::updateHand(ImuObservationsBare observations) {
    ImuState<double> state = imu_.update(observations);
    HandObjective objective = objective_;
    objective.forearm.adjust(Matrix<double, 3, 3>(imu_.getState().orientation), handBasis_.forearm.orientation.normalMatrix());
    instructions_ = HandInstructions(objective, parameter_, instructions_);
}

HandObjective Model::getObjective() const
{
    return objective_;
}

HandInstructions Model::getInstructions()
{
    //instructions_ = HandInstructions(objective_);
    return instructions_;
}

QQuaternion Model::getImu() const
{
    Quat<double> imu = imu_.getState().orientation;
    QQuaternion orientation(imu(0), imu(1), imu(2), imu(3));
    return orientation;
}

double Model::angle(Leap::Matrix a, Leap::Matrix b, Leap::Vector normal) {
    //trouver la paire de vecteur la plus perpendiculaire au normal
    float dotProduct[3];

    dotProduct[0] = a.xBasis.dot(normal) + b.xBasis.dot(normal);
    dotProduct[1] = a.yBasis.dot(normal) + b.yBasis.dot(normal);
    dotProduct[2] = a.zBasis.dot(normal) + b.zBasis.dot(normal);

    //calculer les deux vecteur normal au plan
    Leap::Vector planeNormal[2];
    if (dotProduct[0] < dotProduct[1] && dotProduct[0]  < dotProduct[2]) {
        planeNormal[0] = normal.cross(a.xBasis);
        planeNormal[1] = normal.cross(b.xBasis);
    }
    else if (dotProduct[1] < dotProduct[0] && dotProduct[1] < dotProduct[2]) {
        planeNormal[0] = normal.cross(a.yBasis);
        planeNormal[1] = normal.cross(b.yBasis);
    }
    else {
        planeNormal[0] = normal.cross(a.zBasis);
        planeNormal[1] = normal.cross(b.zBasis);
    }

    //calculer l'angle entre les deux vecteur
    float angle = planeNormal[0].angleTo(planeNormal[1]);

    //caclculer le signe de l'angle
    Leap::Vector normalTest = planeNormal[0].cross(planeNormal[1]);

    if (normal.dot(normalTest) >= 0) {
        return angle;
    }
    else {
        return -angle;
    }
}
