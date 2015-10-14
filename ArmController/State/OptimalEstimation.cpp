#include "OptimalEstimation.h"

using namespace cv;

System::System(Mat x, Mat P)
{
    x_ = x;
    P_ = P;
}

void System::init(Mat x, Mat P)
{
    x_ = x;
    P_ = P;
}

void System::timeUpdate(KalmanUpdateEquation method)
{
    timeUpdate(Range(0, x_.rows), method);
}

void System::timeUpdate(const Range stateRange, KalmanUpdateEquation method)
{

    switch (method) {
    case KalmanUpdateEquation::CLASSIC: {
        Mat F = getF(x_, stateRange);
        Mat Q = getQ(stateRange);
        P_(stateRange, stateRange) = (F * P_(stateRange, stateRange) * F.t()) + Q;
        break;
    }

    default:
        break;
    }
    x_.rowRange(stateRange) = computeTimePropagation(x_, stateRange);
}

void System::measurementUpdate(const Mat y, KalmanUpdateEquation method)
{
    measurementUpdate(y, Range(0, x_.rows), Range(0, y.rows), method);
}

void System::measurementUpdate(const Mat y, const Range stateRange, const Range measurmentRange, KalmanUpdateEquation method)
{
    switch (method) {
    case KalmanUpdateEquation::CLASSIC: {
        Mat H = getH(x_, stateRange, measurmentRange);
        Mat R = getR(measurmentRange);

        Mat M = P_(stateRange, stateRange) * H.t();
        K_ = M * ((H * M) + R).inv();
        P_(stateRange, stateRange) = (Mat::eye(P_(stateRange, stateRange).size(), CV_64F) - (K_ * H)) * P_(stateRange, stateRange);
        stateUpdate(y, stateRange, measurmentRange);
        break;
    }

    default:
        break;
    }
}

void System::constraint(const Mat d, ConstraintWeight weight)
{
    constraint(d, Range(0, x_.rows), Range(0, d.rows), weight);
}

void System::constraint(const Mat d, const Range stateRange, const Range constraintRange, ConstraintWeight weight) {
    Mat W;
    Mat D = getD(stateRange, constraintRange);

    switch (weight) {
    case ConstraintWeight::MINIMUM_VARIANCE:
        W = P_(stateRange, stateRange).inv();
        break;
    case ConstraintWeight::CLOSEST:
        W = Mat::eye(P_(stateRange, stateRange).size(), CV_64F);
        break;
    case ConstraintWeight::CUSTOM:
        W = getW(stateRange);
    }

    x_ = x_ - W.inv() * D.t() * (D * W.inv() * D.t()).inv() * (D*x_ - d);
}

void System::stateUpdate(const Mat y, const Range stateRange, const Range measurmentRange) {
    x_.rowRange(stateRange) += K_ * (y.rowRange(measurmentRange) - computeMeasurementPropagation(x_, measurmentRange));
}

double System::computeRelStateLikelihood(Mat state, Range stateRange)
{
    return 1.0;
}

Mat System::computeTimePropagation(Mat state, Range stateRange)
{
    return state.rowRange(stateRange);
}

Mat System::getQ(Range stateRange)
{
    return Mat::eye( P_(stateRange, stateRange).size(), CV_64F );
}

Mat System::getdQ(Range stateRange) {
    return Mat::eye( P_(stateRange, stateRange).size(), CV_64F );
}

Mat System::getF(Mat state, Range stateRange)
{
    return Mat::eye( P_(stateRange, stateRange).size(), CV_64F );
}

Mat System::getdF(Mat state, Range stateRange)
{
    return Mat::zeros( P_(stateRange, stateRange).size(), CV_64F );
}

Mat System::computeMeasurementPropagation(Mat state, Range measurmentRange)
{
    return state.rowRange(measurmentRange);
}

Mat System::getR(Range measurmentRange)
{
    return Mat::eye( {measurmentRange.size(), measurmentRange.size()}, CV_64F );
}

Mat System::getdR(Range measurmentRange)
{
    return Mat::eye( {measurmentRange.size(), measurmentRange.size()}, CV_64F );
}

Mat System::getH(Mat state, const Range stateRange, const Range measurmentRange)
{
    return Mat::eye( {stateRange.size(), measurmentRange.size()}, CV_64F );
}

Mat System::getD(const Range stateRange, const Range constraintRange)
{
    return Mat::eye( {stateRange.size(), constraintRange.size()}, CV_64F );
}

Mat System::getW(const Range stateRange)
{
    return P_(stateRange, stateRange).inv();
}

