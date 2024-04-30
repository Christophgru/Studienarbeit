#include "stringparser.h"

using json = nlohmann::json;




std::vector<calc::SensorValue> getangles(const std::string& s) {
    std::vector<calc::SensorValue> sensorValues;

    try {
        std::cout<<s<<std::endl;
        size_t lastBracketPos = s.find_last_of('[');

        // Extract the content of the last square brackets
        std::string sl = s.substr(lastBracketPos);
        json j = json::parse(sl);

        for (const auto& item : j) {
            calc::SensorValue value;
            value.theta = item["theta"].get<float>();
            value.val = item["val"].get<float>();
            value.xpos = item["xpos"].get<float>();
            sensorValues.push_back(value);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    }

    return sensorValues;
}