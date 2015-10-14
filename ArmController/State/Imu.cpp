#include "Imu.h"
#include <iomanip>
#include <iostream>

using namespace cv;

ImuSystem::ImuSystem(ImuState<double>& state, ImuParameter<double>& parameter) :
    parameter_(parameter),
    x_(state)
{
    ImuState<double> P = ImuState<double>::getInitialCovariance();
    init(Mat(sizeof(ImuState<double>)/sizeof(double), 1, CV_64F, (void*)&x_),
           Mat::diag(Mat(sizeof(ImuState<double>)/sizeof(double), 1, CV_64F, (void*)&P)).clone());

    Q_ = ImuState<double>::getNoiseCovariance();
    R_ = ImuObservations<double>::getNoiseCovariance();
}

void ImuSystem::update(ImuObservations<double> observations)
{
    timeUpdate();
    measurementUpdate(Mat(sizeof(observations)/sizeof(double), 1, CV_64F, (void*)&observations));
}

Mat ImuSystem::computeTimePropagation(const Mat state, const Range stateRange)
{
    x_ = *(ImuState<double>*)state.data;
    x_ = x_.computeTimeUpdate();
    return Mat(sizeof(ImuState<double>)/sizeof(double), 1, CV_64F, (void*)&x_);
}

Mat ImuSystem::getQ(const Range stateRange)
{
    return Mat::diag(Mat(sizeof(ImuState<double>)/sizeof(double), 1, CV_64F, (void*)&Q_));
}

Mat ImuSystem::getF(const Mat state, const Range stateRange)
{
    x_ = *(ImuState<double>*)state.data;
    F_ = x_.computeTimeUpdateJac();
    const int size[] = {sizeof(ImuState<double>)/sizeof(double), sizeof(ImuState<double>)/sizeof(double)};
    return Mat(2, size, CV_64F, (void*)&F_);
}

Mat ImuSystem::computeMeasurementPropagation(const Mat state, const Range measurmentRange)
{
    x_ = *(ImuState<double>*)state.data;
    y_ = x_.computeObservation(parameter_);
    return Mat(sizeof(ImuObservations<double>)/sizeof(double), 1, CV_64F, (void*)&y_);
}

Mat ImuSystem::getR(const Range measurmentRange)
{
    return Mat::diag(Mat(1, {sizeof(ImuObservations<double>)/sizeof(double)}, CV_64F, (void*)&R_));
}

Mat ImuSystem::getH(const Mat state, const Range stateRange, const Range measurmentRange)
{
    x_ = *(ImuState<double>*)state.data;
    H_ = x_.computeObservationJac(parameter_);
    const int size[] = {sizeof(ImuObservations<double>)/sizeof(double), sizeof(ImuState<double>)/sizeof(double)};
    return Mat(2, size, CV_64F, (void*)&H_);
}

AcclScaleSystem::AcclScaleSystem(ImuState<double>& state, ImuParameter<double>& parameter) :
    state_(state),
    parameter_(parameter)
{
    ImuParameter<double> P = ImuParameter<double>::getInitialCovariance();
    init(Mat(3, 1, CV_64F, (void*)&parameter_.scaleAccl), Mat::diag(Mat(3, 1, CV_64F, (void*)&P.scaleAccl)).clone());
    R_ = ImuObservations<double>::getNoiseCovariance().accl;
}

void AcclScaleSystem::update(ImuObservations<double> observations)
{
    measurementUpdate(Mat(3, 1, CV_64F, (void*)&observations.accl));
}

Mat AcclScaleSystem::computeMeasurementPropagation(const Mat state, const Range measurmentRange)
{
    Matrix<double, 3, 1> accl;
    for (int i = 0; i < 3; i++) {
        accl(i,0) = Matrix<double,3,3>(state_.orientation)(2,i) * parameter_.scaleAccl(i,0) + parameter_.translAccl(i,0);
    }
    return Mat(3, 1, CV_64F, (void*)&accl).clone();
}

Mat AcclScaleSystem::getR(const Range measurmentRange)
{
    return Mat::diag(Mat(3, 1, CV_64F, (void*)&R_));
}

Mat AcclScaleSystem::getH(const Mat state, const Range stateRange, const Range measurmentRange)
{
    Matrix<double, 1, 3> orientation = Matrix<double, 3, 3>(state_.orientation)(2);
    return Mat::diag(Mat(3, 1, CV_64F, (void*)&orientation)).clone();
}

AcclTranslSystem::AcclTranslSystem(ImuState<double>& state, ImuParameter<double>& parameter) :
    state_(state),
    parameter_(parameter)
{
    ImuParameter<double> P = ImuParameter<double>::getInitialCovariance();
    init(Mat(3, 1, CV_64F, (void*)&parameter_.translAccl), Mat::diag(Mat(3, 1, CV_64F, (void*)&P.translAccl)).clone());
    R_ = ImuObservations<double>::getNoiseCovariance().accl;
}

void AcclTranslSystem::update(ImuObservations<double> observations)
{
    measurementUpdate(Mat(3, 1, CV_64F, (void*)&observations.accl));
}

Mat AcclTranslSystem::computeMeasurementPropagation(const Mat state, const Range measurmentRange)
{
    Matrix<double, 3, 1> accl;
    for (int i = 0; i < 3; i++) {
        accl(i,0) = Matrix<double,3,3>(state_.orientation)(2,i) * parameter_.scaleAccl(i,0) + parameter_.translAccl(i,0);
    }
    return Mat(3, 1, CV_64F, (void*)&accl).clone();
}

Mat AcclTranslSystem::getR(const Range measurmentRange)
{
    return Mat::diag(Mat(3, 1, CV_64F, (void*)&R_));
}

MagnScaleSystem::MagnScaleSystem(ImuState<double>& state, ImuParameter<double>& parameter) :
    state_(state),
    parameter_(parameter)
{
    ImuParameter<double> P = ImuParameter<double>::getInitialCovariance();
    init(Mat(3, 1, CV_64F, (void*)&parameter_.scaleMagn), Mat::diag(Mat(3, 1, CV_64F, (void*)&P.scaleMagn)).clone());
    R_ = ImuObservations<double>::getNoiseCovariance().magn;
}

void MagnScaleSystem::update(ImuObservations<double> observations)
{
    measurementUpdate(Mat(3, 1, CV_64F, (void*)&observations.magn));
}

Mat MagnScaleSystem::computeMeasurementPropagation(const Mat state, const Range measurmentRange)
{
    Matrix<double, 3, 1> magn = (Matrix<double, 3, 3>(state_.orientation).t() * parameter_.magnTransition);
    for (int i = 0; i < 3; i++) {
        magn(i,0) = magn(i,0) * parameter_.scaleMagn(i,0) + parameter_.translMagn(i,0);
    }
    return Mat(3, 1, CV_64F, (void*)&magn).clone();
}

Mat MagnScaleSystem::getR(const Range measurmentRange)
{
    return Mat::diag(Mat(3, 1, CV_64F, (void*)&R_));
}

Mat MagnScaleSystem::getH(const Mat state, const Range stateRange, const Range measurmentRange)
{
    Matrix<double, 3, 1> magn = (Matrix<double, 3, 3>(state_.orientation).t() * parameter_.magnTransition);
    return Mat::diag(Mat(3, 1, CV_64F, (void*)&magn)).clone();
}

MagnTranslSystem::MagnTranslSystem(ImuState<double>& state, ImuParameter<double>& parameter) :
    state_(state),
    parameter_(parameter)
{
    ImuParameter<double> P = ImuParameter<double>::getInitialCovariance();
    init(Mat(3, 1, CV_64F, (void*)&parameter_.translMagn), Mat::diag(Mat(3, 1, CV_64F, (void*)&P.translMagn)).clone());
    R_ = ImuObservations<double>::getNoiseCovariance().magn;
}

void MagnTranslSystem::update(ImuObservations<double> observations)
{
    measurementUpdate(Mat(3, 1, CV_64F, (void*)&observations.magn));
}

Mat MagnTranslSystem::computeMeasurementPropagation(const Mat state, const Range measurmentRange)
{
    Matrix<double, 3, 1> magn = (Matrix<double, 3, 3>(state_.orientation).t() * parameter_.magnTransition);
    for (int i = 0; i < 3; i++) {
        magn(i,0) = magn(i,0) * parameter_.scaleMagn(i,0) + parameter_.translMagn(i,0);
    }
    return Mat(3, 1, CV_64F, (void*)&magn).clone();
}

Mat MagnTranslSystem::getR(const Range measurmentRange)
{
    return Mat::diag(Mat(3, 1, CV_64F, (void*)&R_));
}

GyroScaleSystem::GyroScaleSystem(ImuState<double>& state, ImuParameter<double>& parameter) :
    state_(state),
    parameter_(parameter)
{
    ImuParameter<double> P = ImuParameter<double>::getInitialCovariance();
    init(Mat(3, 1, CV_64F, (void*)&parameter_.scaleGyro), Mat::diag(Mat(3, 1, CV_64F, (void*)&P.scaleGyro)).clone());
    R_ = ImuObservations<double>::getNoiseCovariance().gyro;
}

void GyroScaleSystem::update(ImuObservations<double> observations)
{
    measurementUpdate(Mat(3, 1, CV_64F, (void*)&observations.gyro));
}

Mat GyroScaleSystem::computeMeasurementPropagation(const Mat state, const Range measurmentRange)
{
    Matrix<double, 3, 1> gyro;
    for (int i = 0; i < 3; i++) {
        gyro(i,0) = state_.rotation(i,0) * parameter_.scaleGyro(i,0) + parameter_.translGyro(i,0);
    }
    return Mat(3, 1, CV_64F, (void*)&gyro).clone();
}

Mat GyroScaleSystem::getR(const Range measurmentRange)
{
    return Mat::diag(Mat(3, 1, CV_64F, (void*)&R_));
}

Mat GyroScaleSystem::getH(const Mat state, const Range stateRange, const Range measurmentRange)
{
    return Mat::diag(Mat(3, 1, CV_64F, (void*)&state_.rotation)).clone();
}

GyroTranslSystem::GyroTranslSystem(ImuState<double>& state, ImuParameter<double>& parameter) :
    state_(state),
    parameter_(parameter)
{
    ImuParameter<double> P = ImuParameter<double>::getInitialCovariance();
    init(Mat(3, 1, CV_64F, (void*)&parameter_.translGyro), Mat::diag(Mat(3, 1, CV_64F, (void*)&P.translGyro)).clone());
    R_ = ImuObservations<double>::getNoiseCovariance().gyro;
}

void GyroTranslSystem::update(ImuObservations<double> observations)
{
    measurementUpdate(Mat(3, 1, CV_64F, (void*)&observations.gyro));
}

Mat GyroTranslSystem::computeMeasurementPropagation(const Mat state, const Range measurmentRange)
{
    Matrix<double, 3, 1> gyro;
    for (int i = 0; i < 3; i++) {
        gyro(i,0) = state_.rotation(i,0) * parameter_.scaleMagn(i,0) + parameter_.translMagn(i,0);
    }
    return Mat(3, 1, CV_64F, (void*)&gyro).clone();
}

Mat GyroTranslSystem::getR(const Range measurmentRange)
{
    return Mat::diag(Mat(3, 1, CV_64F, (void*)&R_));
}

MagnTransitionSystem::MagnTransitionSystem(ImuState<double>& state, ImuParameter<double>& parameter) :
    state_(state),
    parameter_(parameter)
{
    ImuParameter<double> P = ImuParameter<double>::getInitialCovariance();
    init(Mat(3, 1, CV_64F, (void*)&parameter_.magnTransition), Mat::diag(Mat(3, 1, CV_64F, (void*)&P.magnTransition)).clone());
    for (int i = 0; i < 3; i++) {
        R_(i,0) = ImuObservations<double>::getNoiseCovariance().gyro(i,0);
    }
    R_(3,0) = ImuObservations<double>::getNoiseCovariance().norm;
}

void MagnTransitionSystem::update(ImuObservations<double> observations)
{
    Matrix<double, 4, 1> z;
    for (int i = 0; i < 3; i++) {
        z(i,0) = observations.magn(i,0);
    }
    z(3,0) = observations.norm;
    measurementUpdate(Mat(4, 1, CV_64F, (void*)&z));
}

Mat MagnTransitionSystem::computeMeasurementPropagation(const Mat state, const Range measurmentRange)
{
    Matrix<double, 3, 1> transition = (*(Matrix<double, 3, 1>*)state.data);
    Matrix<double, 3, 1> magn = (Matrix<double, 3, 3>(state_.orientation)).t() * transition;
    for (int i = 0; i < 3; i++) {
        y_(i,0) = magn(i,0) * parameter_.scaleMagn(i,0) + parameter_.translMagn(i,0);
    }
    y_(3,0) = dot(transition, transition);
    return Mat(4, 1, CV_64F, (void*)&y_);
}

Mat MagnTransitionSystem::getR(const Range measurmentRange)
{
    return Mat::diag(Mat(4, 1, CV_64F, (void*)&R_));
}

Mat MagnTransitionSystem::getH(const Mat state, const Range stateRange, const Range measurmentRange)
{
    Matrix<double, 3, 1> transition = (*(Matrix<double, 3, 1>*)state.data);

    for (int i = 0; i < 3; i++) {
        H_(3, i) = 2 * transition(i, 0);
        for (int j = 0;  j < 3; j++) {
            H_(i,j) = (Matrix<double, 3, 3>(state_.orientation))(j,i) * parameter_.scaleMagn(i,0) + parameter_.translMagn(i,0);
        }
    }
    return Mat(4, 3, CV_64F, (void*)&H_);
}


Imu::Imu() :
    state_(ImuState<double>::getInitial()),
    parameter_(ImuParameter<double>::getInitial()),
    imuSystem_(state_, parameter_),
    acclScale_(state_, parameter_),
    acclTransl_(state_, parameter_),
    magnScale_(state_, parameter_),
    magnTransl_(state_, parameter_),
    gyroScale_(state_, parameter_),
    gyroTransl_(state_, parameter_),
    magnTransition_(state_, parameter_)
{
}

ImuState<double> Imu::update(ImuObservationsBare observationBare)
{
    ImuObservations<double> observations(observationBare);

    imuSystem_.update(observations);

    acclScale_.update(observations);
    acclTransl_.update(observations);
    magnScale_.update(observations);
    magnTransl_.update(observations);
    gyroScale_.update(observations);
    gyroTransl_.update(observations);
    magnTransition_.update(observations);

    return state_;
}
