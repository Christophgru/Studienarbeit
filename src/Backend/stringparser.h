#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iostream>  
#include <regex>
#include <nlohmann/json.hpp>
#include "calc.h"

std::vector<double> getAnglesFromString(std::string str);


std::vector<calc::SensorValue> getangles(const std::string& s) ;