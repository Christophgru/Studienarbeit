#include "calc.h"
#include <cassert>



using namespace calc;
    




int testdistance1(){

    line l({0,0},{1,0});
    point p({0,0});
    float result =    calc::getDistance(p,l);
    float expectedResult =  0;
    
    // Assert that the result matches the expected result
    assert(result == expectedResult);
    
    std::cout << "Test dist1 passed!" << std::endl;


}
int testdistance2(){

    line l({0,0},{1,0});
    point p({0,0});
    float result =    calc::getDistance(p,l);
    float expectedResult =  0;
    
    // Assert that the result matches the expected result
    assert(result == expectedResult);
    
    std::cout << "Test dist2 passed!" << std::endl;


}
int testdistance3(){

    line l({0,0},{1,1});
    point p({0,0});
    float result =    calc::getDistance(p,l);
    float expectedResult =  0;
    
    // Assert that the result matches the expected result
    assert(result == expectedResult);
    
    std::cout << "Test dist3 passed!" << std::endl;


}

int testdistance4(){

    line l({0,0},{0,1});
    point p({1,0});
    float result =    calc::getDistance(p,l);
    float expectedResult =  1;
    
    // Assert that the result matches the expected result
    assert(result == expectedResult);
    
    std::cout << "Test dist4 passed!" << std::endl;
}

int testdistance5(){

    line l({0,0},{1,1});
    point p({2,2});
    float result =    calc::getDistance(p,l);
    float expectedResult = 0 ;
    
    // Assert that the result matches the expected result
    assert(result == expectedResult);
    
    std::cout << "Test dist5 passed!" << std::endl;
}

int testdistance6(){

    line l({0,0},{1,1});
    point p({-1,1});
    float result =    calc::getDistance(p,l);
    float expectedResult = sqrt(2) ;
    
    // Assert that the result matches the expected result
    assert(result == expectedResult);
    
    std::cout << "Test dist6 passed!" << std::endl;
}

int testdistance7(){

    line l({-2,-2},{1,1});
    point p({-1,1});
    float result =    calc::getDistance(p,l);
    float expectedResult = sqrt(2);
    
    // Assert that the result matches the expected result
    assert(result == expectedResult);
    
    std::cout << "Test dist7 passed!" << std::endl;
}

int testdistance8(){

    line l({-2,-2},{1,1});
    point p({-1,1});
    float result =    calc::getDistance(p,l);
    float expectedResult = sqrt(2) ;
    
    // Assert that the result matches the expected result
    assert(result == expectedResult);
    
    std::cout << "Test dist8 passed!" << std::endl;
}


int main(int argc, char const *argv[])
{

try
{
    testdistance1();
    testdistance2();
    testdistance3();
    testdistance4();
    testdistance5();
    testdistance6();
    testdistance7();
    testdistance8();
}
catch(const std::exception& e)
{
    std::cerr << e.what() << '\n';
}


   
   
}