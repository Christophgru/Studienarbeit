/**
 * @file calc.h
 * @brief Header file for the calc library.
 * 
 * This file contains the definitions of classes and functions used for various calculations 
 * such as distance measurement, gradient descent, and vector operations.
 * 
 * @author Christoph Gruender
 * @date 2024-05-17
 */

#ifndef STUDIENARBEIT_CALC_H
#define STUDIENARBEIT_CALC_H

#define DEBUGLEVEL 1

#include <vector>
#include <cmath>
#include <iostream>

/**
 * @namespace calc
 * @brief Namespace for calculation algorithms like angle computation or mean value calculation.
 */
namespace calc {

/**
 * @brief Data class for the result of point calculation methods.
 */
struct point {
    std::vector<double> position; ///< Position of the point
    std::vector<double> uncertainty; ///< Uncertainty of the point's position

    /**
     * @brief Constructor for point.
     * @param f Position of the point.
     * @param Uncertainty Uncertainty of the point's position.
     */
    point(std::vector<double> f, std::vector<double> uncertainty={0} );

    /**
     * @brief Sets the x-coordinate of the point.
     * @param x The x-coordinate.
     */
    void setx(double x);

    /**
     * @brief Gets the x-coordinate of the point.
     * @return The x-coordinate.
     */
    double getx();

    /**
     * @brief Sets the y-coordinate of the point.
     * @param y The y-coordinate.
     */
    void sety(double y);

    /**
     * @brief Gets the y-coordinate of the point.
     * @return The y-coordinate.
     */
    double gety();
};

/**
 * @brief Data class representing a line.
 */
struct line {
    std::vector<double> start; ///< Start point of the line
    std::vector<double> direction; ///< Direction vector of the line
    double uncertainty;

    /**
     * @brief Constructor for line.
     * @param start Start point of the line.
     * @param direction Direction vector of the line.
     */
    line(std::vector<double> start, std::vector<double> direction, double uncertainty=0.0);

    /**
     * @brief Gets a point on the line.
     * @return A point on the line.
     */
    std::vector<double> getPointOnLine();
};

/**
 * @brief Data class representing a sensor value.
 */
struct SensorValue {
    double theta; ///< Angle of the sensor
    double val; ///< Sensor output value
    std::vector<double> pos; ///< x-position of the sensor
};

/**
 * @brief Calculates the position from sensor angles.
 * @param sensorData Vector of sensor values.
 * @param lastPoint The last calculated point.
 * @return The calculated point.
 */
point getPosFromAngles(std::vector<calc::SensorValue> sensorData, calc::point lastPoint);

double distance(point p, line l);
double meanDistance(point p, std::vector<line> lines);
double sqMeanDistance(point p, std::vector<line> lines);
unsigned char doubleToUnsignedCharInRange(double value);
bool almostEqual(double a, double b, double epsilon = 1e-10);
point gradientDescent(point startingpoint, std::vector<line> lines, double gamma = .5, int steps = 20, double delta = 1e-15);
double maxUncertainty(point p, std::vector<line> l);
double dotProduct(const std::vector<double>& v1, const std::vector<double>& v2);
std::vector<double> vectorSubtract(const std::vector<double>& v1, const std::vector<double>& v2);
std::vector<double> scalarMultiply(const std::vector<double>& v, double scalar);
double vectorNorm(const std::vector<double>& v);

} // namespace calc

#endif // STUDIENARBEIT_CALC_H
