#include "ArmKnowledge.h"

int16_t swapBytes(int16_t value) {
    int8_t * bytes = (int8_t*)&value;
    return (bytes[0]<<8) + uint8_t(bytes[1]);
}

JointInstructions::JointInstructions(JointObjective objective, JointParameter& parameter, JointInstructions instructions)
{
    double tmp = (objective.position - parameter.transl)/parameter.scale * 32768;
    if (parameter.speed * (tmp - instructions.position) < -OFFSET_THRESHOLD) {
        tmp += sgn(tmp - instructions.position) * parameter.offset;
    }

    position = tmp;

    if (tmp >= 32767) {
        position = 32767;
    }
    else if (tmp < -32768) {
        position = -32768;
    }
    parameter.speed = position - instructions.position;
}

FingerInstructions::FingerInstructions(FingerObjective objective, FingerParameter& parameter, FingerInstructions instructions)
{
    for (int i = 0; i < 4; i++) {
        joints[i] = JointInstructions(objective.joints[i], parameter.joints[i], instructions.joints[i]);
    }
}

ForearmInstructions::ForearmInstructions(ForearmObjective objective, ForearmParameter& parameter, ForearmInstructions instructions)
{
    for (int i = 0; i < 2; i++) {
        wristFlexion[i] = JointInstructions(objective.wristFlexion[i], parameter.wristFlexion[i], instructions.wristFlexion[i]);
    }
}

HandInstructions::HandInstructions(HandObjective objective, HandParameter& parameter, HandInstructions instructions)
{
    forearm = ForearmInstructions(objective.forearm, parameter.forearm, instructions.forearm);
    for (int i = 0; i < 5; i++) {
        fingers[i] = FingerInstructions(objective.fingers[i], parameter.fingers[i], instructions.fingers[i]);
    }
}

void ForearmObjective::adjust(Matrix<double, 3, 3> state, Matrix<double, 3, 3> objective)
{
    Matrix<double, 3, 1> normal[3];
    for (int i = 0; i < 3; i++) {
        normal[i](i,0) = 1.0;
    }

    Matrix<double,3,1> tilt;
    tilt(0,0) = objective(2,1);
    tilt(1,0) = objective(0,1);
    tilt(2,0) = objective(1,1);

    Matrix<double, 3, 1> planeNormal[3][2];
    for (int i = 0; i < 3; i++) {
        planeNormal[i][0] = cross(normal[i], state(2).t());
        planeNormal[i][1] = cross(normal[i], tilt);
    }

    Matrix<double, 3, 1> normalTest = cross(planeNormal[1][0], planeNormal[1][1]);
    if (dot(normal[1], normalTest) < 0) {
        wristFlexion[0].position += asin(norm(cross(planeNormal[1][0], planeNormal[1][1]))/norm(planeNormal[1][0])) * norm(planeNormal[1][0]);
    }
    else {
        wristFlexion[0].position -= asin(norm(cross(planeNormal[1][0], planeNormal[1][1]))/norm(planeNormal[1][0])) * norm(planeNormal[1][0]);
    }

    normalTest = cross(planeNormal[2][0], planeNormal[2][1]);
    if (dot(normal[2], normalTest) < 0) {
        wristFlexion[1].position += asin(norm(cross(planeNormal[2][0], planeNormal[2][1]))/norm(planeNormal[2][0])) * norm(planeNormal[2][0]);
    }
    else {
        wristFlexion[1].position -= asin(norm(cross(planeNormal[2][0], planeNormal[2][1]))/norm(planeNormal[2][0])) * norm(planeNormal[2][0]);
    }
}

HandParameter::HandParameter()
{
    forearm.wristFlexion[0].scale = WRIST_FLEXION_SCALE[0];
    forearm.wristFlexion[0].transl = WRIST_FLEXION_TRANSL[0];
    forearm.wristFlexion[0].offset = WRIST_FLEXION_TRANSL[0];
    forearm.wristFlexion[1].scale = WRIST_FLEXION_SCALE[1];
    forearm.wristFlexion[1].transl = WRIST_FLEXION_TRANSL[1];
    forearm.wristFlexion[1].offset = WRIST_FLEXION_OFFSET[1];

    fingers[0].joints[0].scale = THUMB_TRAPEZIUM_SCALE;
    fingers[0].joints[0].transl = THUMB_TRAPEZIUM_TRANSL;
    fingers[0].joints[0].offset = THUMB_TRAPEZIUM_OFFSET;

    fingers[0].joints[1].scale = THUMB_METACARPAL_SCALE;
    fingers[0].joints[1].transl = THUMB_METACARPAL_TRANSL;
    fingers[0].joints[1].offset = THUMB_METACARPAL_OFFSET;

    fingers[0].joints[2].scale = THUMB_PROXIMAL_SCALE;
    fingers[0].joints[2].transl = THUMB_PROXIMAL_TRANSL;
    fingers[0].joints[2].offset = THUMB_PROXIMAL_OFFSET;

    fingers[0].joints[3].scale = THUMB_DISTAL_SCALE;
    fingers[0].joints[3].transl = THUMB_DISTAL_TRANSL;
    fingers[0].joints[3].offset = THUMB_DISTAL_OFFSET;

    for (int i = 0; i < 4; i++) {
        fingers[i+1].joints[0].scale = FINGER_JOINT_SCALE[i];
        fingers[i+1].joints[0].transl = FINGER_JOINT_SCALE[i];
        fingers[i+1].joints[0].offset = FINGER_JOINT_OFFSET[i];

        fingers[i+1].joints[1].scale = FINGER_PROXIMAL_SCALE[i];
        fingers[i+1].joints[1].transl = FINGER_PROXIMAL_TRANSL[i];
        fingers[i+1].joints[1].offset = FINGER_PROXIMAL_OFFSET[i];

        fingers[i+1].joints[2].scale = FINGER_DISTAL_SCALE[i];
        fingers[i+1].joints[2].transl = FINGER_DISTAL_TRANSL[i];
        fingers[i+1].joints[2].offset = FINGER_DISTAL_OFFSET[i];

        fingers[i+1].joints[3].scale = FINGER_DISTAL_SCALE[i];
        fingers[i+1].joints[3].transl = FINGER_DISTAL_TRANSL[i];
        fingers[i+1].joints[3].offset = FINGER_DISTAL_OFFSET[i];
    }
}
