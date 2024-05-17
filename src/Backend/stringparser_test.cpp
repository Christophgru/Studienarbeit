/**
 * @file stringparser_test.cpp
 * @brief Unit tests for the string parsing functions.
 * 
 * This file contains unit tests for the functions that parse sensor values from JSON strings.
 * 
 * @author Christoph Gruender
 * @date 2024-05-17
 */

#include "stringparser.h"
#include <cassert>
#include <iostream>

/**
 * @brief Test function for getangles.
 * 
 * Tests the getangles function with a sample JSON string.
 */
void test_getangles() {
    std::string jsonString = R"([{"theta": 45.0, "val": 10.0, "xpos": 5.0},
                                  {"theta": 90.0, "val": 20.0, "xpos": 10.0}])";

    std::vector<calc::SensorValue> sensorValues = getangles(jsonString);

    // Check the size of the result
    assert(sensorValues.size() == 2);

    // Check the values of the first sensor value
    assert(sensorValues[0].theta == 45.0);
    assert(sensorValues[0].val == 10.0);
    assert(sensorValues[0].xpos == 5.0);

    // Check the values of the second sensor value
    assert(sensorValues[1].theta == 90.0);
    assert(sensorValues[1].val == 20.0);
    assert(sensorValues[1].xpos == 10.0);

    if(DEBUGLEVEL) std::cout << "Test for getangles passed!" << std::endl;
}

int main() {
    test_getangles();
    return 0;
}
