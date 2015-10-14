#ifndef STATE_ESTIMATION_H
#define STATE_ESTIMATION_H

#include <stdio.h>
#include "State/ArmKnowledge.h"
#include "State/OptimalEstimation.h"
#include "Average.h"


class ImuSystem : public System {
public:
    ImuSystem() = default;
    ImuSystem(ImuState<double>& state, ImuParameter<double>& parameter);

    void update(ImuObservations<double> observations);
    
protected:
    virtual cv::Mat computeTimePropagation(const cv::Mat state, const cv::Range stateRange);
    virtual cv::Mat getQ(const cv::Range stateRange);
    virtual cv::Mat getF(const cv::Mat state, const cv::Range stateRange);

    virtual cv::Mat computeMeasurementPropagation(const cv::Mat state, const cv::Range measurmentRange);
    virtual cv::Mat getR(const cv::Range measurmentRange);
    virtual cv::Mat getH(const cv::Mat state, const cv::Range stateRange, const cv::Range measurmentRange);
    
private:
    ImuParameter<double>& parameter_;

    ImuState<double>& x_;
    ImuObservations<double> y_;

    ImuState<double> Q_;
    ImuState<ImuState<double> > F_;

    ImuObservations<double> R_;
    ImuObservations<ImuState<double> > H_;
};


class AcclScaleSystem : public System {
public:
    AcclScaleSystem() = default;
    AcclScaleSystem(ImuState<double>& state, ImuParameter<double>& parameter);

    void update(ImuObservations<double> observations);

protected:
    virtual cv::Mat computeMeasurementPropagation(const cv::Mat state, const cv::Range measurmentRange);
    virtual cv::Mat getR(const cv::Range measurmentRange);
    virtual cv::Mat getH(const cv::Mat state, const cv::Range stateRange, const cv::Range measurmentRange);

private:
    ImuState<double>& state_;
    ImuParameter<double>& parameter_;
    Matrix<double, 3, 1> R_;


};

class AcclTranslSystem : public System {
public:
    AcclTranslSystem() = default;
    AcclTranslSystem(ImuState<double>& state, ImuParameter<double>& paramater);

    void update(ImuObservations<double> observations);

protected:
    virtual cv::Mat computeMeasurementPropagation(const cv::Mat state, const cv::Range measurmentRange);
    virtual cv::Mat getR(const cv::Range measurmentRange);

private:
    ImuState<double>& state_;
    ImuParameter<double>& parameter_;
    Matrix<double, 3, 1> R_;
};

class MagnScaleSystem : public System {
public:
    MagnScaleSystem() = default;
    MagnScaleSystem(ImuState<double>& state, ImuParameter<double>& paramater);

    void update(ImuObservations<double> observations);

protected:
    virtual cv::Mat computeMeasurementPropagation(const cv::Mat state, const cv::Range measurmentRange);
    virtual cv::Mat getR(const cv::Range measurmentRange);
    virtual cv::Mat getH(const cv::Mat state, const cv::Range stateRange, const cv::Range measurmentRange);

private:
    ImuState<double>& state_;
    ImuParameter<double>& parameter_;
    Matrix<double, 3, 1> R_;
};

class MagnTranslSystem : public System {
public:
    MagnTranslSystem() = default;
    MagnTranslSystem(ImuState<double>& state, ImuParameter<double>& paramater);

    void update(ImuObservations<double> observations);

protected:
    virtual cv::Mat computeMeasurementPropagation(const cv::Mat state, const cv::Range measurmentRange);
    virtual cv::Mat getR(const cv::Range measurmentRange);

private:
    ImuState<double>& state_;
    ImuParameter<double>& parameter_;
    Matrix<double, 3, 1> R_;
};

class GyroScaleSystem : public System {
public:
    GyroScaleSystem() = default;
    GyroScaleSystem(ImuState<double>& state, ImuParameter<double>& paramater);

    void update(ImuObservations<double> observations);

protected:
    virtual cv::Mat computeMeasurementPropagation(const cv::Mat state, const cv::Range measurmentRange);
    virtual cv::Mat getR(const cv::Range measurmentRange);
    virtual cv::Mat getH(const cv::Mat state, const cv::Range stateRange, const cv::Range measurmentRange);

private:
    ImuState<double>& state_;
    ImuParameter<double>& parameter_;
    Matrix<double, 3, 1> R_;
};

class GyroTranslSystem : public System {
public:
    GyroTranslSystem() = default;
    GyroTranslSystem(ImuState<double>& state, ImuParameter<double>& paramater);

    void update(ImuObservations<double> observations);

protected:
    virtual cv::Mat computeMeasurementPropagation(const cv::Mat state, const cv::Range measurmentRange);
    virtual cv::Mat getR(const cv::Range measurmentRange);

private:
    ImuState<double>& state_;
    ImuParameter<double>& parameter_;
    Matrix<double, 3, 1> R_;
};

class MagnTransitionSystem : public System {
public:
    MagnTransitionSystem() = default;
    MagnTransitionSystem(ImuState<double>& state, ImuParameter<double>& paramater);

    void update(ImuObservations<double> observations);

protected:
    virtual cv::Mat computeMeasurementPropagation(const cv::Mat state, const cv::Range measurmentRange);
    virtual cv::Mat getR(const cv::Range measurmentRange);
    virtual cv::Mat getH(const cv::Mat state, const cv::Range stateRange, const cv::Range measurmentRange);

private:
    ImuState<double>& state_;
    ImuParameter<double>& parameter_;
    Matrix<double, 4, 1> y_;
    Matrix<double, 4, 1> R_;
    Matrix<double, 4, 3> H_;
};

class Imu {
public:
    Imu();
    
    ImuState<double> update(ImuObservationsBare observationBare);
    ImuState<double> getState() const {return state_;}
    
private:
    ImuState<double> state_;
    ImuParameter<double> parameter_;
    ImuSystem imuSystem_;

    AcclScaleSystem acclScale_;
    AcclTranslSystem acclTransl_;

    MagnScaleSystem magnScale_;
    MagnTranslSystem magnTransl_;

    GyroScaleSystem gyroScale_;
    GyroTranslSystem gyroTransl_;

    MagnTransitionSystem magnTransition_;
};

#endif /* defined(__ArmController__StateEstimation__) */
