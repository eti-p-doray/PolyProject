#ifndef ARMKNOWLEDGE_H
#define ARMKNOWLEDGE_H

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <QMatrix4x4>
#include "Matrix.h"

#include <iostream>
#include <iomanip>

const double MAX_CURRENT = 100;

/* MOTOR PARARMETER */

const double OFFSET_THRESHOLD = 100.0;

const double WRIST_FLEXION_SCALE[2] = {1.35263017, 0.6981317008};
const double WRIST_FLEXION_TRANSL[2] = {0.2181661565, 0.0};
const double WRIST_FLEXION_OFFSET[2] = {0.0, 0.0};

const double THUMB_TRAPEZIUM_SCALE = 0.2617993878;
const double THUMB_TRAPEZIUM_TRANSL = 0.1745329252;
const double THUMB_TRAPEZIUM_OFFSET = 0.0;
const double THUMB_METACARPAL_SCALE = 0.9861110274;
const double THUMB_METACARPAL_TRANSL = 0.2879793266;
const double THUMB_METACARPAL_OFFSET = 0.0;
const double THUMB_PROXIMAL_SCALE = 0.7417649321;
const double THUMB_PROXIMAL_TRANSL = 0.4799655443;
const double THUMB_PROXIMAL_OFFSET = 0.0;
const double THUMB_DISTAL_SCALE = 1.221730476;
const double THUMB_DISTAL_TRANSL = 0.436332313;
const double THUMB_DISTAL_OFFSET = 0.0;

const double FINGER_JOINT_SCALE[4] = {0.3490658504, 0.6981317008, 0.3490658504, 0.6981317008};
const double FINGER_JOINT_TRANSL[4] = {0.0, 0.0, 0.0, 0.0};
const double FINGER_JOINT_OFFSET[4] = {0.0, 0.0, 0.0, 0.0};
const double FINGER_PROXIMAL_SCALE[4] = {0.9773843811, -0.8988445648, 1.134464014, 1.178097245};
const double FINGER_PROXIMAL_TRANSL[4] = {-0.6806784083, -0.7592182246, -0.6108652382, -0.6544984695};
const double FINGER_PROXIMAL_OFFSET[4] = {0.0, 0.0, 0.0, 0.0};
const double FINGER_DISTAL_SCALE[4] = {1.021017612, 1.160643953, 1.021017612, 1.160643953};
const double FINGER_DISTAL_TRANSL[4] = {-0.7592182246, -0.8988445648, -0.7592182246, -0.8988445648};
const double FINGER_DISTAL_OFFSET[4] = {0.0, 0.0, 0.0, 0.0};

/* IMU PARAMETER */

const double IMU_PARAMETER_ACCL_SCALE[3][1] = {{-16610.1}, {-16425.85}, {16500.75}};
const double IMU_PARAMETER_MAGN_SCALE[3][1] = {{-777.122}, {789.383}, {-769.756}};
const double IMU_PARAMETER_GYRO_SCALE[3][1] = {{-1000.0*180.0/(8.75*3.14159*1.5e-2)}, {-1000.0*180.0/(8.75*3.14159*1.5e-2)}, {1000.0*180.0/(8.75*3.14159*1.5e-2)}};

const double IMU_PARAMETER_ACCL_TRANSL[3][1] = {{66.3}, {48.55}, {-297.55}};
const double IMU_PARAMETER_MAGN_TRANSL[3][1] = {{-132.224}, {32.658}, {39.626}};
const double IMU_PARAMETER_GYRO_TRANSL[3][1] = {{46.4298}, {10.5205}, {30.3486}};

const double IMU_PARAMETER_MAGN_TRANSITION[3][1] = {{-0.3465593004}, {-0.1561798436e-1}, {0.9378980381}};

const double IMU_PARAMETER_ACCL_SCALE_VAR[3][1] = {{1e2}, {1e2}, {1e2}};
const double IMU_PARAMETER_MAGN_SCALE_VAR[3][1] = {{1e-1}, {1e-1}, {1e-1}};
const double IMU_PARAMETER_GYRO_SCALE_VAR[3][1] = {{1e3}, {1e3}, {1e3}};

const double IMU_PARAMETER_ACCL_TRANSL_VAR[3][1] = {{1e0}, {1e0}, {1e0}};
const double IMU_PARAMETER_MAGN_TRANSL_VAR[3][1] = {{1e-2}, {1e-2}, {1e-2}};
const double IMU_PARAMETER_GYRO_TRANSL_VAR[3][1] = {{1e-2}, {1e-2}, {1e-2}};

const double IMU_PARAMETER_MAGN_TRANSITION_VAR[3][1] = {{1e6}, {1e6}, {1e-8}};

/* IMU STATE */

const double IMU_STATE_INITIAL_ORIENTATION_VAR[4] = {1e-10, 1e-10, 1e-10, 1e-10};
const double IMU_STATE_INITIAL_ROTATION_VAR[3][1] = {{1e-10}, {1e-10}, {1e-10}};

const double IMU_STATE_ORIENTATION_VAR[4] = {1e-8, 1e-8, 1e-8, 1e-8};
const double IMU_STATE_ROTATION_VAR[3][1] = {{1e-7}, {1e-7}, {1e-7}};

/* IMU OBSERVATION */

const double IMU_OBSERVATION_ACCL_VAR[3][1] = {{3054.87},    {2383.13},    {4341.62}};
const double IMU_OBSERVATION_MAGN_VAR[3][1] = {{5.27808},    {12.688},    {10.387}};
const double IMU_OBSERVATION_GYRO_VAR[3][1] = {{615.184},    {976.792},    {654.249}};
const double IMU_OBSERVATION_NORM_VAR = 1e-10;

struct Basis;

struct JointInstructions;
struct JointObjective;
struct JointParameter;

struct FingerInstructions;
struct FingerObjective;
struct FingerParameter;
struct FingerBasis;

struct ForearmInstructions;
struct ForearmObjective;
struct ForearmParameter;

struct HandInstructions;
struct HandObjective;
struct HandParameter;
struct HandBasis;

struct ImuObservationsBare;
template<typename T> struct ImuParameter;
template<typename T> struct ImuObservations;
template<typename T> struct ImuState;


int16_t swapBytes(int16_t value);

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

struct Basis {
    QMatrix4x4 orientation;
    QVector3D position;
    QVector3D scale;
};

struct JointInstructions {
    JointInstructions() {}
    JointInstructions(JointObjective objective, JointParameter& parameter, JointInstructions instructions);

    int16_t position = 0;
};
struct JointObjective {
    double position = 0.0;
};
struct JointParameter {
    double scale = 0.0;
    double transl = 0.0;
    double offset = 0.0;
    double speed = 0.0;
};


struct FingerInstructions {
    FingerInstructions() {}
    FingerInstructions(FingerObjective objective, FingerParameter& parameter, FingerInstructions instructions);

    JointInstructions joints[4];
};
struct FingerObjective {
    JointObjective joints[4];
};
struct FingerParameter {
    JointParameter joints[3];
};
struct FingerBasis {
    Basis joints[4];
};


struct ForearmInstructions {
    ForearmInstructions() {}
    ForearmInstructions(ForearmObjective objective, ForearmParameter& parameter, ForearmInstructions instructions);

    JointInstructions wristFlexion[2];
};
struct ForearmObjective {
    void adjust(Matrix<double, 3, 3> state, Matrix<double, 3, 3> objective);

    JointObjective wristFlexion[2];
};
struct ForearmParameter {
    JointParameter wristFlexion[2];
};


struct HandInstructions {
    HandInstructions() {}
    HandInstructions(HandObjective objective, HandParameter& parameter, HandInstructions instructions);

    ForearmInstructions forearm;
    FingerInstructions fingers[5];
};
struct HandObjective {
    ForearmObjective forearm;
    FingerObjective fingers[5];
};
struct HandParameter {
    HandParameter();
    ForearmParameter forearm;
    FingerParameter fingers[5];
};
struct HandBasis {
    Basis forearm;
    Basis palm;
    FingerBasis fingers[5];
};


struct ImuObservationsBare {
    int16_t accl[3];
    int16_t magn[3];
    int16_t gyro[3];
};
template<typename T> struct ImuParameter {
    static ImuParameter<T> getInitial();
    static ImuParameter<T> getInitialCovariance();
    
    Matrix<T,3,1> scaleAccl = {};
    Matrix<T,3,1> translAccl = {};
    Matrix<T,3,1> scaleMagn = {};
    Matrix<T,3,1> translMagn = {};
    Matrix<T,3,1> scaleGyro = {};
    Matrix<T,3,1> translGyro = {};
    Matrix<T,3,1> magnTransition = {};
};
template<typename T> struct ImuObservations {
    ImuObservations() = default;
    ImuObservations(ImuObservationsBare observations);
    static ImuObservations<T> getNoiseCovariance();

    Matrix<T, 3, 1> accl = {};
    Matrix<T, 3, 1> magn = {};
    Matrix<T, 3, 1> gyro = {};
    T norm = {};
};
template<typename T> struct ImuState {
    static ImuState<T> getInitial();
    static ImuState<T> getInitialCovariance();
    static ImuState<T> getNoiseCovariance();
    
    ImuState<T> computeTimeUpdate();
    ImuState<ImuState<T> > computeTimeUpdateJac();
    ImuObservations<T> computeObservation(ImuParameter<T> imuParameter);
    ImuObservations<ImuState<T> > computeObservationJac(ImuParameter<T> imuParameter);
    
    Quat<T> orientation = {};
    Matrix<T, 3, 1> rotation = {};
};


template <typename T>
ImuState<T> ImuState<T>::getInitial() {
    ImuState<T> state = {};
    state.orientation(0) = 1.0;
    return state;
}

template <typename T>
ImuState<T> ImuState<T>::getInitialCovariance() {
    ImuState<T> covariance = {};

    covariance.orientation = IMU_STATE_INITIAL_ORIENTATION_VAR;
    covariance.rotation = IMU_STATE_INITIAL_ROTATION_VAR;
    return covariance;
}

template <typename T>
ImuState<T> ImuState<T>::getNoiseCovariance() {
    ImuState<T> covariance = {};

    covariance.orientation = IMU_STATE_ORIENTATION_VAR;
    covariance.rotation = IMU_STATE_ROTATION_VAR;
    return covariance;
}

template <typename T>
ImuObservations<T> ImuObservations<T>::getNoiseCovariance() {
    ImuObservations<T> observations = {};
    
    observations.accl = IMU_OBSERVATION_ACCL_VAR;
    observations.magn = IMU_OBSERVATION_MAGN_VAR;
    observations.gyro = IMU_OBSERVATION_GYRO_VAR;
    observations.norm = IMU_OBSERVATION_NORM_VAR;
    return observations;
}

template <typename T>
ImuParameter<T> ImuParameter<T>::getInitial() {
    ImuParameter<T> parameter;
    
    parameter.scaleAccl = IMU_PARAMETER_ACCL_SCALE;
    parameter.scaleMagn = IMU_PARAMETER_MAGN_SCALE;
    parameter.scaleGyro = IMU_PARAMETER_GYRO_SCALE;
    
    parameter.translAccl = IMU_PARAMETER_ACCL_TRANSL;
    parameter.translMagn = IMU_PARAMETER_MAGN_TRANSL;
    parameter.translGyro = IMU_PARAMETER_GYRO_TRANSL;
    
    parameter.magnTransition = IMU_PARAMETER_MAGN_TRANSITION;
    
    return parameter;
}

template <typename T>
ImuParameter<T> ImuParameter<T>::getInitialCovariance() {
    ImuParameter<T> covariance = {};
    
    covariance.scaleAccl = IMU_PARAMETER_ACCL_SCALE_VAR;
    covariance.scaleMagn = IMU_PARAMETER_MAGN_SCALE_VAR;
    covariance.scaleGyro = IMU_PARAMETER_GYRO_SCALE_VAR;
    
    covariance.translAccl = IMU_PARAMETER_ACCL_TRANSL_VAR;
    covariance.translMagn = IMU_PARAMETER_MAGN_TRANSL_VAR;
    covariance.translGyro = IMU_PARAMETER_GYRO_TRANSL_VAR;
    
    covariance.magnTransition = IMU_PARAMETER_MAGN_TRANSITION_VAR;
    
    return covariance;
}

template <typename T>
ImuObservations<T>::ImuObservations(ImuObservationsBare observations)
{
    accl(0,0) = float((observations.accl[0]));
    magn(0,0) = float(swapBytes(observations.magn[2]));
    gyro(0,0) = float((observations.gyro[0]));

    accl(1,0) = float((observations.accl[1]));
    magn(1,0) = float(swapBytes(observations.magn[0]));
    gyro(1,0) = float((observations.gyro[1]));

    accl(2,0) = float((observations.accl[2]));
    magn(2,0) = float(swapBytes(observations.magn[1]));
    gyro(2,0) = float((observations.gyro[2]));

    norm = 1.0;
}

template <typename T>
ImuState<T> ImuState<T>::computeTimeUpdate() {
    ImuState<T> state;
    
    Quat<T> rotationQuat;
    rotationQuat(0) = 0.0;
    rotationQuat(1) = rotation(0,0);
    rotationQuat(2) = rotation(1,0);
    rotationQuat(3) = rotation(2,0);
    Quat<T> orientationDiff = orientation*rotationQuat*0.5;
    state.orientation = orientation + orientationDiff;

    state.rotation = rotation;
 
    return state;
}

template <typename T>
ImuState<ImuState<T> > ImuState<T>::computeTimeUpdateJac() {
    ImuState<ImuState<T> > jac = {};
    
    Quat<T> rotationQuat;
    rotationQuat(0) = 0.0;
    rotationQuat(1) = rotation(0,0);
    rotationQuat(2) = rotation(1,0);
    rotationQuat(3) = rotation(2,0);
    
    Matrix<T, 4, 4> orientationDiffFromOrientationJac = rotationQuat.getMulPostMatrix()*0.5;
    Matrix<T, 4, 4> orientationDiffFromRotationJac = orientation.getMulPreMatrix()*0.5;
    
    for (int i = 0; i < 4; i++) {
        jac.orientation(i).orientation(i) = 1.0;
        for (int j = 0; j < 4; j++) {
            jac.orientation(i).orientation(j) += orientationDiffFromOrientationJac(i,j);
        }
    }
    for (int j = 0; j < 3; j++) {
        jac.rotation(j,0).rotation(j,0) = 1.0;
        for (int i = 0; i < 4; i++) {
            jac.orientation(i).rotation(j,0) = orientationDiffFromRotationJac(i,j);
        }
    }
    
    return jac;
}

template<typename T>
ImuObservations<T> ImuState<T>::computeObservation(ImuParameter<T> imuParameter) {
    ImuObservations<T> observation;
    
    Matrix<T, 3, 3> orientationMat = Matrix<T, 3, 3>(orientation);
    observation.magn = orientationMat.t() * imuParameter.magnTransition;

    for (int i = 0; i < 3; i++) {
        observation.accl(i,0) = (imuParameter.scaleAccl(i,0) * orientationMat(2,i)) + imuParameter.translAccl(i,0);
        observation.magn(i,0) = (imuParameter.scaleMagn(i,0) * observation.magn(i,0)) + imuParameter.translMagn(i,0);
        observation.gyro(i,0) = (imuParameter.scaleGyro(i,0) * rotation(i,0)) + imuParameter.translGyro(i,0);
    }
    
    observation.norm = orientation.norm();

    return observation;
}

template <typename T>
ImuObservations<ImuState<T> > ImuState<T>::computeObservationJac(ImuParameter<T> imuParameter) {
    ImuObservations<ImuState<T> > jac = {};
    
    Matrix<T, 3, 3> matrixConversionJac[4];
    for (int i = 0; i < 4; i++) {
        matrixConversionJac[i] = orientation.getMatrixConversionJac(i);
    }
    
    Matrix<T, 3, 1> magnFromOrientationJac[4];
    for (int i = 0; i < 4; i++) {
        magnFromOrientationJac[i] = matrixConversionJac[i].t() * imuParameter.magnTransition;
    }
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            jac.accl(i,0).orientation(j) = matrixConversionJac[j](2,i) * imuParameter.scaleAccl(i,0);
            jac.magn(i,0).orientation(j) = magnFromOrientationJac[j](i,0) * imuParameter.scaleMagn(i,0);
        }
        jac.gyro(i,0).rotation(i,0) = imuParameter.scaleGyro(i,0);
    }
    for (int i = 0; i < 4; i++) {
        jac.norm.orientation(i) += 2 * orientation(i);
    }
    
    return jac;
}

#endif
