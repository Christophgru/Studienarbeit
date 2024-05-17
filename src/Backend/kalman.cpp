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

calc::point KalmanFilter::filter(calc::point input){
    //do crazy kalman shit here :)
    if(history.size()==queueSize){
        history.pop_back();
        }
    history.push_front(input);
    calc::point output=input;
    return output;
};

KalmanFilter::KalmanFilter(/* args */)
{
}

KalmanFilter::~KalmanFilter()
{
}