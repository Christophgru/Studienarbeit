/**
 * @file kalman.cpp
 * @brief 
 * 
 * 
 * 
 * @author Jonathan Simon
 * @date 2024-05-17
 */

#include "kalman.h"
#include <iostream>
#include <cmath>

static constexpr size_t DIM_X{2};
static constexpr size_t DIM_Z{2};
static kf::KalmanFilter<DIM_X, DIM_Z> kalmanfilter;

kf::Vector<2> convertCartesian2Polar(const kf::Vector<2>& cartesian);
kf::Matrix<DIM_Z, DIM_Z> calculateJacobianMatrix(const kf::Vector<DIM_X>& vecX);
Eigen::Vector2f executeCorrectionStep(const calc::point& input);



namespace kf{



kf::Vector<2> convertCartesian2Polar(const kf::Vector<2>& cartesian)
{
    const kf::Vector<2> polar{
        std::sqrt(cartesian[0] * cartesian[0] + cartesian[1] * cartesian[1]),
        std::atan2(cartesian[1], cartesian[0])
    };
    return polar;
}

kf::Matrix<DIM_Z, DIM_Z> calculateJacobianMatrix(const kf::Vector<DIM_X>& vecX)
{
    const kf::float32_t valX2PlusY2{ (vecX[0] * vecX[0]) + (vecX[1] * vecX[1]) };
    const kf::float32_t valSqrtX2PlusY2{ std::sqrt(valX2PlusY2) };

    kf::Matrix<DIM_Z, DIM_Z> matHj;
    matHj <<
        (vecX[0] / valSqrtX2PlusY2), (vecX[1] / valSqrtX2PlusY2),
        (-vecX[1] / valX2PlusY2), (vecX[0] / valX2PlusY2);

    return matHj;
}

Eigen::Vector2f executeCorrectionStep(const calc::point& input)
{
    kalmanfilter.vecX() << 0.1F, 0.1F;             // Prediction State   
    kalmanfilter.matP() << 0.3F, 0.0F, 0.0F, 0.3F;   // Covariance

    kf::Vector<2> measPosCart;
    measPosCart << input.position[0], input.position[1];
    const kf::Vector<DIM_Z> vecZ{ convertCartesian2Polar(measPosCart) };

    kf::Matrix<DIM_Z, DIM_Z> matR;
    matR << 0.1F, 0.0F, 0.0F, 0.1F;

    kf::Matrix<DIM_Z, DIM_X> matHj{ calculateJacobianMatrix(kalmanfilter.vecX()) }; // Jacobian Matrix Hj

    kalmanfilter.correctEkf(convertCartesian2Polar, vecZ, matR, matHj);

    std::cout << "\ncorrected state vector = \n" << kalmanfilter.vecX() << "\n";
    std::cout << "\ncorrected state covariance = \n" << kalmanfilter.matP() << "\n";

    Eigen::Vector2f correctedPosition;
    correctedPosition[0] = kalmanfilter.vecX()[0];
    correctedPosition[1] = kalmanfilter.vecX()[1];

    return correctedPosition;
}



calc::point filter(calc::point input)
{
    // Ensure input.position is sized correctly
    if (input.position.size() != 2) {
        input.position.resize(2);
    }

    // Execute the correction step and update input.position
    Eigen::Vector2f correctedPosition = executeCorrectionStep(input);
    input.position[0] = correctedPosition[0];
    input.position[1] = correctedPosition[1];

    return input;
}
}