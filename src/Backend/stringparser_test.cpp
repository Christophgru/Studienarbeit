#include "stringparser.h"
#include <cassert>

int main(int argc, char const *argv[])
{
    std::string input = "[9, -70,0,120000]";
    std::vector<float> result = getAnglesFromString(input);
    std::vector<float> expectedResult = {9, -70, 0, 120000};
    
    // Assert that the result matches the expected result
    assert(result == expectedResult);
    
    std::cout << "Test passed!" << std::endl;

    return 0;
}
