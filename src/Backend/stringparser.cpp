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
        if(DEBUGLEVEL);
        std::cout<<std::endl<<"received string:" << s << std::endl;
        std::string begin_msg="[{";
        size_t lastBracketPos = s.rfind(begin_msg.c_str());
        if (lastBracketPos == std::string::npos) {
            std::cout<<"invalid json"<<std::endl;
            throw std::invalid_argument("Invalid JSON string format");
        }

        // Extract the content of the last square brackets
        std::string sl = s.substr(lastBracketPos);
        std::cout<<"short:"<<sl<<std::endl;
        json j = json::parse(sl);

        for (const auto& item : j) {
            std::cout<<"parse";
            calc::SensorValue value;
            value.theta = item["theta"].get<double>();
            value.val = item["val"].get<double>();
            for (const auto& posValue : item["pos"]) {
                value.pos.push_back(posValue.get<double>());
            }
            sensorValues.push_back(value);
        }
        std::cout<<"done";
    } catch (const json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
    } catch (const json::type_error& e) {
        std::cerr << "JSON type error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    }

    return sensorValues;
}
