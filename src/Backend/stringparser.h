/**
 * @file stringparser.h
 * @brief Header file for the string parsing functions.
 * 
 * This file contains the declarations of functions for parsing sensor values from JSON strings.
 * 
 * @author Christoph Gruender
 * @date 2024-05-17
 */

#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <regex>
#include <nlohmann/json.hpp>
#include "calc.h"

/**
 * @brief Parses angles from a string.
 * 
 * @param str The input string containing angles.
 * @return std::vector<double> A vector of parsed angles.
 */
std::vector<double> getAnglesFromString(std::string str);

/**
 * @brief Parses sensor values from a JSON string.
 * 
 * @param s The JSON string containing sensor values.
 * @return std::vector<calc::SensorValue> A vector of parsed sensor values.
 */
std::vector<calc::SensorValue> getangles(const std::string& s);
