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
#include <deque>

class KalmanFilter
{
private:
    //add persitantstuff here
    std::deque<calc::point> history;
    int queueSize=5;
public:
    KalmanFilter();
    ~KalmanFilter();
    
calc::point filter(calc::point input);
};





#endif // STUDIENARBEIT_kalman_H