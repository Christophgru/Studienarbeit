#include "calc.h"
#include <cassert>


bool almostEqual(double a, double b, double epsilon = 1e-6) {
    return std::abs(a - b) < epsilon;
}


int main(int argc, char const *argv[])
{
    line l1(0,0);
    point p1(0,0);
    float result1 =    calc::getDistance(p1,l1);
    float expectedResult1 =  0;
    
    // Assert that the result matches the expected result
    assert(result1 == expectedResult1);
    
    std::cout << "Test 1 passed!" << std::endl;



    line l2(5,40);
    point p2(0,0);
    float result2 =    calc::getDistance(p2,l2);
    float expectedResult2 =  5;
    
    std::cout<<result2<<std::endl;
    // Assert that the result matches the expected result
    assert(almostEqual(result2,expectedResult2,0.1));
    
    std::cout << "Test 2 passed!" << std::endl;



    line l3(-2000,0.001);
    point p3(5,-2);
    float result3 =    calc::getDistance(p3,l3);
    float expectedResult3 =  4;
    
    std::cout<<result3<<std::endl;
    // Assert that the result matches the expected result
    assert(almostEqual(result3,expectedResult3,0.001));
    
    std::cout << "Test 3 passed!" << std::endl;


     line l4(-2,1);
    point p4(0,0);
    float result4 =    calc::getDistance(p4,l4);
    float expectedResult4 =  sqrt(2);
    
    std::cout<<result4<<std::endl;
    // Assert that the result matches the expected result
    assert(almostEqual(result4,expectedResult4,0.001));
    
    std::cout << "Test 4 passed!" << std::endl;
    

    return 0;
}