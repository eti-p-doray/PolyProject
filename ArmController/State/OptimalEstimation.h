#ifndef OPTIMAL_ESTIMATION_H
#define OPTIMAL_ESTIMATION_H

#include <stdio.h>
#include <opencv2/core/core.hpp>

//joseph Stabilisation
//kalman
//postCovariance
//information
//sequential


enum class KalmanUpdateEquation {CLASSIC};
enum class ConstraintWeight {MINIMUM_VARIANCE, CLOSEST, CUSTOM};

class System {
public:
    System() = default;
    System(cv::Mat x, cv::Mat P);
    void init(cv::Mat x, cv::Mat P);

    void timeUpdate(KalmanUpdateEquation method = KalmanUpdateEquation::CLASSIC);
    void timeUpdate(const cv::Range stateRange, KalmanUpdateEquation method = KalmanUpdateEquation::CLASSIC);
    //void timeUpdate(const cv::Range stateRange, double dt);

    void measurementUpdate(const cv::Mat y, KalmanUpdateEquation method = KalmanUpdateEquation::CLASSIC);
    void measurementUpdate(const cv::Mat y, const cv::Range stateRange, const cv::Range measurmentRange, KalmanUpdateEquation method = KalmanUpdateEquation::CLASSIC);

    void constraint(const cv::Mat d, ConstraintWeight weight = ConstraintWeight::MINIMUM_VARIANCE);
    void constraint(const cv::Mat d, const cv::Range stateRange, const cv::Range constraintRange, ConstraintWeight weight = ConstraintWeight::MINIMUM_VARIANCE);

    cv::Mat getState() {return x_;}

protected:
    virtual cv::Mat computeTimePropagation(const cv::Mat state, const cv::Range stateRange);
    //virtual cv::Mat computeTimePropagation(const cv::Mat state, const cv::Range stateRange, double dt);//seconds
    virtual cv::Mat getQ(const cv::Range stateRange);
    virtual cv::Mat getdQ(const cv::Range stateRange);
    virtual cv::Mat getF(const cv::Mat state, const cv::Range stateRange);
    virtual cv::Mat getdF(const cv::Mat state, const cv::Range stateRange);

    virtual cv::Mat computeMeasurementPropagation(const cv::Mat state, const cv::Range measurmentRange);
    virtual cv::Mat getR(const cv::Range measurmentRange);
    virtual cv::Mat getdR(const cv::Range measurmentRange);
    virtual cv::Mat getH(const cv::Mat state, const cv::Range stateRange, const cv::Range measurmentRange);

    virtual cv::Mat getD(const cv::Range stateRange, const cv::Range constraintRange);
    virtual cv::Mat getW(const cv::Range stateRange);

    virtual double computeRelStateLikelihood(const cv::Mat state, const cv::Range stateRange);
    
private:
    cv::Mat x_;
    cv::Mat P_;
    cv::Mat K_;
    
    void stateUpdate(const cv::Mat y, const cv::Range stateRange, const cv::Range measurmentRange);
};

#endif /* defined(__ArmController__OptimalEsticv::Mation__) */
