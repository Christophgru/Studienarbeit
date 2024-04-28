#include "calc.h"
#include <cassert>



using namespace calc;
    




int testdistance1(){

    line l({0,0},{1,0});
    point p({0,0});
    float result =    calc::distance(p,l);
    float expectedResult =  0;
    
    // Assert that the result matches the expected result
    assert(result == expectedResult);
    
    std::cout << "Test dist1 passed!" << std::endl;


}
int testdistance2(){

    line l({0,0},{1,0});
    point p({0,0});
    float result =    calc::distance(p,l);
    float expectedResult =  0;
    
    // Assert that the result matches the expected result
    assert(result == expectedResult);
    
    std::cout << "Test dist2 passed!" << std::endl;


}
int testdistance3(){

    line l({0,0},{1,1});
    point p({0,0});
    float result =    calc::distance(p,l);
    float expectedResult =  0;
    
    // Assert that the result matches the expected result
    assert(result == expectedResult);
    
    std::cout << "Test dist3 passed!" << std::endl;


}

int testdistance4(){

    line l({0,0},{0,1});
    point p({1,0});
    float result =    calc::distance(p,l);
    float expectedResult =  1;
    
    // Assert that the result matches the expected result
    assert(result == expectedResult);
    
    std::cout << "Test dist4 passed!" << std::endl;
}

int testdistance5(){

    line l({0,0},{1,1});
    point p({2,2});
    float result =    calc::distance(p,l);
    float expectedResult = 0 ;
    
    // Assert that the result matches the expected result
    assert(result == expectedResult);
    
    std::cout << "Test dist5 passed!" << std::endl;
}

int testdistance6(){

    line l({0,0},{1,1});
    point p({-1,1});
    float result =    calc::distance(p,l);
    float expectedResult = sqrt(2) ;
    
    // Assert that the result matches the expected result
    assert(result == expectedResult);
    
    std::cout << "Test dist6 passed!" << std::endl;
}

int testdistance7(){

    line l({-2,-2},{1,1});
    point p({-1,1});
    float result =    calc::distance(p,l);
    float expectedResult = sqrt(2);
    
    // Assert that the result matches the expected result
    assert(result == expectedResult);
    
    std::cout << "Test dist7 passed!" << std::endl;
}

int testdistance8(){

    line l({0,0},{1,0});
    for (int i = 0; i < 20; i++)
    {
    float number=static_cast<float>(i)/10.0f;
    point p({0,number});
    float result =    calc::distance(p,l);
    
    float expectedResult = number ;
    
    // Assert that the result matches the expected result
    assert(result == expectedResult);
    }
    
    
    
    std::cout << "Test dist8 passed!" << std::endl;
}



int testgrad1(){
    line l1({0,2},{1,0});
    std::vector<line> lines={l1};
    point sp({0,0});
    point res=gradientDescent(sp,lines);
    point expectedRes=point({0,2});
    assert(almostEqual(res.getx(),expectedRes.getx(),0.001f));
    assert(almostEqual(res.gety(),expectedRes.gety(),0.001f));
    std::cout << "Test grad1 passed!" << std::endl;
}

int testgrad2(){
    line l2({2,0},{0,1});
    std::vector<line> lines={l2};
    point sp({0,0});
    point res=gradientDescent(sp,lines);
    point expectedRes=point({2,0});
    assert(almostEqual(res.getx(),expectedRes.getx(),0.001f));
    assert(almostEqual(res.gety(),expectedRes.gety(),0.001f));
    
    std::cout << "Test grad2 passed!" << std::endl;
}

int testgrad3(){
    line l2({2,0},{0,1});
    line l1({0,2},{1,0});
    std::vector<line> lines={l1,l2};
    point sp({0,0});
    point res=gradientDescent(sp,lines);
    point expectedRes=point({2,2});
    assert(almostEqual(res.getx(),expectedRes.getx(),0.001f));
    assert(almostEqual(res.gety(),expectedRes.gety(),0.001f));
    
    std::cout << "Test grad3 passed!" << std::endl;
}


int testgrad4(){
    line l2({0,0},{1,1.75});
    line l1({0,3},{-1,1.66});
    std::vector<line> lines={l1,l2};
    point sp({0,0});
    point res=gradientDescent(sp,lines);
    point expectedRes=point({2,2});
    assert(almostEqual(res.getx(),expectedRes.getx(),0.001f));
    assert(almostEqual(res.gety(),expectedRes.gety(),0.001f));
    
    std::cout << "Test grad3 passed!" << std::endl;
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
    std::cerr <<"error in distance algo"<< e.what() << '\n';
}

try
{
    testgrad1();
    testgrad2();
    testgrad3();
}
catch(const std::exception& e)
{
    std::cerr <<"error in lin prog algo"<< e.what() << '\n';
}


   
   
}