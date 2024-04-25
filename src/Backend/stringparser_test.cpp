#include "stringparser.h"
#include <cassert>
#include "calc.h"





void test1(){
    std::string jsonString = R"([{"theta": 0, "val": 61.93038398982172, "xpos": 0}, {"theta": 0, "val": 61.90184169488966, "xpos": 3}])";
    std::vector<calc::SensorValue> angles = getangles(jsonString);

    // Output the parsed values
    for (const auto& angle : angles) {
        std::cout << "Theta: " << angle.theta << ", Val: " << angle.val << ", Xpos: " << angle.xpos << std::endl;
    }
}
int main() {

    test1();

    return 0;
}