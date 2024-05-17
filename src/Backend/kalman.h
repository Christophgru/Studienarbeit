/**
 * @file kalman.h
 * @brief 
 * 
 * 
 * 
 * @author Jonathan Simon
 * @date 2024-05-17
 */


#ifndef STUDIENARBEIT_kalman_H
#define STUDIENARBEIT_kalman_H


#include "calc.h"

class KalmanFilter
{
private:
    //add persitantstuff here
    std::vector<calc::point> history;
public:
    KalmanFilter();
    ~KalmanFilter();
    
calc::point filter(calc::point input);
};





#endif // STUDIENARBEIT_kalman_H