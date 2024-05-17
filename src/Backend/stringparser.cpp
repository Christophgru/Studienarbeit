/**
 * @file stringparser.cpp
 * @brief Implementation file for the string parsing functions.
 * 
 * This file contains the implementation of functions for parsing sensor values from JSON strings.
 * 
 * @author Christoph Gruender
 * @date 2024-05-17
 */

#include "stringparser.h"
using json = nlohmann::json;

/**
 * @brief Parses sensor values from a JSON string.
 * 
 * @param s The JSON string containing sensor values.
 * @return std::vector<calc::SensorValue> A vector of parsed sensor values.
 */
std::vector<calc::SensorValue> getangles(const std::string& s) {
    std::vector<calc::SensorValue> sensorValues;

    try {
        if(DEBUGLEVEL) std::cout << s << std::endl;
        size_t lastBracketPos = s.find_last_of('[');

        // Extract the content of the last square brackets
        std::string sl = s.substr(lastBracketPos);
        json j = json::parse(sl);

        for (const auto& item : j) {
            calc::SensorValue value;
            value.theta = item["theta"].get<double>();
            value.val = item["val"].get<double>();
            value.xpos = item["xpos"].get<double>();
            sensorValues.push_back(value);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    }

    return sensorValues;
}
