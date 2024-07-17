/**
 * @file main.cpp
 * @brief Main file for the application.
 * 
 * This file contains the main function that initializes and runs the application,
 * including reading sensor data, calculating positions, and projecting positions.
 * 
 * @author Christoph Gruender
 * @date 2024-05-17
 */

#define DEBUGLEVEL 1


#include "socket.h"
#include "iostream"
#include "calc.h"
#include "display.h"
#include <vector>
#include "stringparser.h"
#include "kalman.h"
#include <chrono>

/**
 * @brief Main function for the application.
 * 
 * Initializes the display and socket management, reads sensor data,
 * calculates positions, and projects positions.
 * 
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return int Execution status.
 */
int main(int argc, char** argv) {
    display* d = new display();
    SocketVerwaltung s = SocketVerwaltung();
    kf::KalmanFilter <2,2>kalman;
    calc::point lastValue = calc::point({0.0, 0.0});

    while (true) {
        std::string string = "";
        int red = s.read(&string);
        auto start = std::chrono::high_resolution_clock::now(); // Record the start time

        if (0 < red) {
            std::vector<calc::SensorValue> angles = getangles(string);
            if(DEBUGLEVEL) std::cout << std::endl;
            for (calc::SensorValue s : angles) {  
                if(DEBUGLEVEL) std::cout << "xpos: " << s.pos[0]<<"ypos: "<< s.pos[1] << " theta: " << s.theta << " val: " << s.val << "; ";
            }
            if(DEBUGLEVEL) std::cout << std::endl;
            lastValue = calc::getPosFromAngles(angles, lastValue);
            lastValue=kf::filter(lastValue);
            d->projectPos(angles, lastValue);
        } else if (red != 0) {
            printf("error");
            break;
        }

        auto end = std::chrono::high_resolution_clock::now(); // Record the end time
        std::chrono::duration<double> elapsed = end - start;  // Calculate the elapsed time
        std::cout << "Time taken for this round: " << elapsed.count() << " seconds" << std::endl;
    }
    return 0;
}
