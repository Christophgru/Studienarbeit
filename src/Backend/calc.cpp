/**
 * @file calc.cpp
 * @brief Implementation file for the calc library.
 * 
 * This file contains the implementation of various functions for distance measurement,
 * gradient descent, and vector operations.
 * 
 * @author Christoph Gruender
 * @date 2024-05-17
 */

#include "calc.h"
#include <cassert>
#include <iterator>
#include <algorithm>

namespace calc {

/**
 * @brief Calculates the position from sensor angles.
 * @param sensorData Vector of sensor values.
 * @param lastPoint The last calculated point.
 * @return The calculated point.
 */
point getPosFromAngles(std::vector<SensorValue> sensorData, point lastPoint) {
    std::vector<line> lines;
    for (SensorValue value : sensorData) {
        double angle = value.theta + value.val;
        double angle_rad = angle / 180 * M_PI;
        std::vector<double> coord = value.pos;
        if(DEBUGLEVEL) {std::cout << " theta:" << value.theta << " sensor_out: " << value.val << " [angle: " << sinf(angle_rad) << "/" << angle << " pos: " ;
            std::copy(coord.begin(), coord.end(), std::ostream_iterator<double>(std::cout, " "));
            std::cout << "] ";}
        if(DEBUGLEVEL) std::cout << "[" << cosf(angle_rad) << "|" << sinf(angle_rad) << "]\n";
        double uncertainty_angle=5e-11*pow(value.val,6)+5.1;
        line l = line(coord, {cosf(angle_rad), sinf(angle_rad)}, uncertainty_angle);
        lines.push_back(l);
    }
    if (std::isnan(lastPoint.getx())) {
        lastPoint.setx(0.0);
    }
    if (std::isnan(lastPoint.gety())) {
        lastPoint.sety(0.0);
    }

    point p = gradientDescent(lastPoint, lines);
    

    if(DEBUGLEVEL) std::cout << "calculated pos (" << p.getx() << "|" << p.gety() << ")" << std::endl << std::endl << std::endl;

    p.uncertainty=std::vector<double>(p.position.size(),maxUncertainty(p,lines));
    return p;
}

/**
 * @brief Calculates highest expected uncertainty between point and 2nd most precise line.
 * @param p point.
 * @param lines lines with uncertainty.
 * @return highest expected diffrence.
 */
double maxUncertainty(point p, std::vector<line> lines){
    if(lines.size()<2)return 0;
    std::vector<double> uncertainties;
    for(line l : lines){
        double angle =atan(l.direction[1]/l.direction[0]);
        double highangle=angle+l.uncertainty;
        double lowangle=angle-l.uncertainty;
        line highline=line(l.start, {cosf(highangle), sinf(highangle)});
        line lowline=line(l.start, {cosf(lowangle), sinf(lowangle)});

        double disthigh=distance(p,highline);
        double distlow=distance(p, lowline);

        double biggerdiff=(disthigh>distlow)?disthigh:distlow;
        uncertainties.push_back(biggerdiff);
    }
    std::sort(uncertainties.begin(), uncertainties.end(), [](double a, double b) {
        return a < b;
    });
    return uncertainties[1];//return second smallest uncertainty
    
};

/**
 * @brief Checks if two doubles are almost equal.
 * @param a First double.
 * @param b Second double.
 * @param epsilon Tolerance for comparison.
 * @return True if the doubles are almost equal, false otherwise.
 */
bool almostEqual(double a, double b, double epsilon) {
    return std::fabs(a - b) < epsilon;
}

/**
 * @brief Gets a point on the line.
 * @return A point on the line.
 */
std::vector<double> line::getPointOnLine() {
    std::vector<double> result(this->start.size());
    for (int i = 0; i < this->start.size(); i++) {
        result[i] = this->start[i] + this->direction[i];
    }
    return result;
}

/**
 * @brief Calculates the dot product of two vectors.
 * @param v1 First vector.
 * @param v2 Second vector.
 * @return The dot product.
 */
double dotProduct(const std::vector<double>& v1, const std::vector<double>& v2) {
    double sum = 0.0;
    for (int i = 0; i < v1.size(); ++i) {
        sum += v1[i] * v2[i];
    }
    return sum;
}

/**
 * @brief Subtracts one vector from another.
 * @param v1 First vector.
 * @param v2 Second vector.
 * @return The result of the subtraction.
 */
std::vector<double> vectorSubtract(const std::vector<double>& v1, const std::vector<double>& v2) {
    std::vector<double> result(v1.size());
    for (int i = 0; i < v1.size(); ++i) {
        result[i] = v1[i] - v2[i];
    }
    return result;
}

/**
 * @brief Multiplies a vector by a scalar.
 * @param v Vector.
 * @param scalar Scalar value.
 * @return The result of the multiplication.
 */
std::vector<double> scalarMultiply(const std::vector<double>& v, double scalar) {
    std::vector<double> result(v.size());
    for (int i = 0; i < v.size(); ++i) {
        result[i] = v[i] * scalar;
    }
    return result;
}

/**
 * @brief Calculates the norm of a vector.
 * @param v Vector.
 * @return The norm of the vector.
 */
double vectorNorm(const std::vector<double>& v) {
    return std::sqrt(dotProduct(v, v));
}

/**
 * @brief Calculates the distance between a point and a line.
 * @param pnt Point.
 * @param l Line.
 * @return The distance between the point and the line.
 */
double distance(point pnt, line l) {
    std::vector<double> A = l.start;
    std::vector<double> B = l.getPointOnLine();
    std::vector<double> P = pnt.position;

    std::vector<double> AP = vectorSubtract(P, A);
    std::vector<double> AB = vectorSubtract(B, A);

    double t = dotProduct(AP, AB) / dotProduct(AB, AB);
    std::vector<double> projection = scalarMultiply(AB, t);
    std::vector<double> d = vectorSubtract(AP, projection);
    return vectorNorm(d);
}

/**
 * @brief Calculates the mean distance between a point and a set of lines.
 * @param p Point.
 * @param lines Vector of lines.
 * @return The mean distance.
 */
double meanDistance(point p, std::vector<line> lines) {
    double dist = 0.0;
    for (line l : lines) {
        dist += distance(p, l);
    }
    return dist / static_cast<double>(lines.size());
}

/**
 * @brief Calculates the squared mean distance between a point and a set of lines.
 * @param p Point.
 * @param lines Vector of lines.
 * @return The squared mean distance.
 */
double sqMeanDistance(point p, std::vector<line> lines) {
    double dist = 0.0;
    for (line l : lines) {
        dist += pow(distance(p, l), 2.0);
    }
    return dist / static_cast<double>(lines.size());
}

/**
 * @brief Constructor for line.
 * @param start Start point of the line.
 * @param direction Direction vector of the line.
 */
line::line(std::vector<double> start, std::vector<double> direction, double uncertainty) {
    this->direction = direction;
    this->start = start;
    this->uncertainty=uncertainty;
}

/**
 * @brief Constructor for point.
 * @param f Position of the point.
 * @param Uncertainty Uncertainty of the point's position.
 */
point::point(std::vector<double> f, std::vector<double> uncertainty) {
    position = f;
    if(uncertainty.size()==1 &&uncertainty[0]==0){//if uncertainty was not passed
        this->uncertainty=std::vector<double>(f.size()); //initialize uncertainty in all dimensions with 0 
    }else{
        this->uncertainty = uncertainty;
    }
}

/**
 * @brief Gets the x-coordinate of the point.
 * @return The x-coordinate.
 */
double point::getx() {
    return this->position[0];
}

/**
 * @brief Sets the x-coordinate of the point.
 * @param x The x-coordinate.
 */
void point::setx(double x) {
    this->position[0] = x;
}

/**
 * @brief Gets the y-coordinate of the point.
 * @return The y-coordinate.
 */
double point::gety() {
    return this->position[1];
}

/**
 * @brief Sets the y-coordinate of the point.
 * @param y The y-coordinate.
 */
void point::sety(double y) {
    this->position[1] = y;
}

/**
 * @brief Converts a double to an unsigned char within the range [0, 255].
 * @param value The double value.
 * @return The converted unsigned char.
 */
unsigned char doubleToUnsignedCharInRange(double value) {
    if (value < 0.0) {
        return 0;
    } else if (value > 255.0) {
        return 255;
    } else {
        return static_cast<unsigned char>(value);
    }
}

/**
 * @brief Performs gradient descent to find the point with the minimum distance to a set of lines.
 * @param startingPoint The starting point for gradient descent.
 * @param lines Vector of lines.
 * @param gamma Learning rate.
 * @param steps Number of steps.
 * @param delta Small increment for numerical differentiation.
 * @return The point with the minimum distance to the lines.
 */
point gradientDescent(point startingPoint, std::vector<line> lines, double gamma, int steps, double delta) {
    if (steps == 0 || std::isnan(startingPoint.getx()) || std::isnan(startingPoint.gety())) {
        return startingPoint;
    }
    double dist = sqMeanDistance(startingPoint, lines);
    double deltaDistX = dist - sqMeanDistance(point({startingPoint.getx() + delta, startingPoint.gety()}), lines);
    double incX = deltaDistX / delta;
    double deltaDistY = dist - sqMeanDistance(point({startingPoint.getx(), startingPoint.gety() + delta}), lines);
    double incY = deltaDistY / delta;
    startingPoint.setx(startingPoint.getx() + gamma * incX);
    startingPoint.sety(startingPoint.gety() + gamma * incY);
    return gradientDescent(startingPoint, lines, gamma, steps - 1);
}

} // namespace calc
