#include "stringparser.h"

std::vector<float> getAnglesFromString(std::string str){
    //std::cout<<"string:\""<<str<<"\"";
    std::vector<float>numbers;
    std::string token;
  
    
        std::regex numberRegex(R"(-?\d+)");

    // Iterate over each match in the input string
    auto it = std::sregex_iterator(str.begin(), str.end(), numberRegex);
    while (it != std::sregex_iterator()) {
        // Get the matched number and convert it to an integer
        std::smatch match = *it;
        try {
            numbers.push_back(std::stoi(match.str())+90);
        } catch (...) {
            std::cerr << "Error parsing token: " << match.str() << std::endl;
        }
        ++it;
    }
    return numbers;
    

};