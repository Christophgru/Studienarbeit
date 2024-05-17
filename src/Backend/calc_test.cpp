/**
 * @file calc_test.cpp
 * @brief Unit tests for the calc library.
 * 
 * This file contains unit tests for the distance and gradient descent functions 
 * from the calc library. 
 * 
 * @author Christoph Gruender
 * @date 2024-05-17
 */

#include "calc.h"
#include <cassert>
#include <iostream>

using namespace calc;

/**
 * @brief Tests the distance calculation between a point and a line (Test case 1).
 */
void testdistance1() {
    line l({0, 0}, {1, 0});
    point p({0, 0});
    double result = calc::distance(p, l);
    double expectedResult = 0;

    // Assert that the result matches the expected result
    assert(result == expectedResult);

    if(DEBUGLEVEL) std::cout << "Test dist1 passed!" << std::endl;
}

/**
 * @brief Tests the distance calculation between a point and a line (Test case 2).
 */
void testdistance2() {
    line l({0, 0}, {1, 0});
    point p({0, 0});
    double result = calc::distance(p, l);
    double expectedResult = 0;

    // Assert that the result matches the expected result
    assert(result == expectedResult);

    if(DEBUGLEVEL) std::cout << "Test dist2 passed!" << std::endl;
}

/**
 * @brief Tests the distance calculation between a point and a line (Test case 3).
 */
void testdistance3() {
    line l({0, 0}, {1, 1});
    point p({0, 0});
    double result = calc::distance(p, l);
    double expectedResult = 0;

    // Assert that the result matches the expected result
    assert(result == expectedResult);

    if(DEBUGLEVEL) std::cout << "Test dist3 passed!" << std::endl;
}

/**
 * @brief Tests the distance calculation between a point and a line (Test case 4).
 */
void testdistance4() {
    line l({0, 0}, {0, 1});
    point p({1, 0});
    double result = calc::distance(p, l);
    double expectedResult = 1;

    // Assert that the result matches the expected result
    assert(result == expectedResult);

    if(DEBUGLEVEL) std::cout << "Test dist4 passed!" << std::endl;
}

/**
 * @brief Tests the distance calculation between a point and a line (Test case 5).
 */
void testdistance5() {
    line l({0, 0}, {1, 1});
    point p({2, 2});
    double result = calc::distance(p, l);
    double expectedResult = 0;

    // Assert that the result matches the expected result
    assert(result == expectedResult);

    if(DEBUGLEVEL) std::cout << "Test dist5 passed!" << std::endl;
}

/**
 * @brief Tests the distance calculation between a point and a line (Test case 6).
 */
void testdistance6() {
    line l({0, 0}, {1, 1});
    point p({-1, 1});
    double result = calc::distance(p, l);
    double expectedResult = sqrt(2);

    // Assert that the result matches the expected result
    assert(result == expectedResult);

    if(DEBUGLEVEL) std::cout << "Test dist6 passed!" << std::endl;
}

/**
 * @brief Tests the distance calculation between a point and a line (Test case 7).
 */
void testdistance7() {
    line l({-2, -2}, {1, 1});
    point p({-1, 1});
    double result = calc::distance(p, l);
    double expectedResult = sqrt(2);

    // Assert that the result matches the expected result
    assert(result == expectedResult);

    if(DEBUGLEVEL) std::cout << "Test dist7 passed!" << std::endl;
}

/**
 * @brief Tests the distance calculation between points on the y-axis and a horizontal line.
 */
void testdistance8() {
    line l({0, 0}, {1, 0});
    for (int i = 0; i < 20; i++) {
        double number = static_cast<double>(i) / 10.0f;
        point p({0, number});
        double result = calc::distance(p, l);
        double expectedResult = number;

        // Assert that the result matches the expected result
        assert(result == expectedResult);
    }

    if(DEBUGLEVEL) std::cout << "Test dist8 passed!" << std::endl;
}

/**
 * @brief Tests the gradient descent algorithm (Test case 1).
 */
void testgrad1() {
    line l1({0, 2}, {1, 0});
    std::vector<line> lines = {l1};
    point sp({0, 0});
    point expectedRes = point({0, 2});

    point res = gradientDescent(sp, lines, 0.5f, 40, 1.0e-20f);

    if(DEBUGLEVEL) std::cout << "res:[" << res.getx() << "|" << 2.0f - res.gety() << "] exp. res:[" << expectedRes.getx() << "|" << expectedRes.gety() << "]";
    assert(almostEqual(res.getx(), expectedRes.getx(), 3e-2f));
    assert(almostEqual(res.gety(), expectedRes.gety(), 3e-2f));
    if(DEBUGLEVEL) std::cout << "Test grad1 passed!" << std::endl;
}

/**
 * @brief Tests the gradient descent algorithm (Test case 2).
 */
void testgrad2() {
    line l2({2, 0}, {0, 1});
    std::vector<line> lines = {l2};
    point sp({0, 0});
    point res = gradientDescent(sp, lines);
    point expectedRes = point({2, 0});
    assert(almostEqual(res.getx(), expectedRes.getx(), 0.001f));
    assert(almostEqual(res.gety(), expectedRes.gety(), 0.001f));

    if(DEBUGLEVEL) std::cout << "Test grad2 passed!" << std::endl;
}

/**
 * @brief Tests the gradient descent algorithm (Test case 3).
 */
void testgrad3() {
    line l2({2, 0}, {0, 1});
    line l1({0, 2}, {1, 0});
    std::vector<line> lines = {l1, l2};
    point sp({0, 0});
    point res = gradientDescent(sp, lines);
    point expectedRes = point({2, 2});
    assert(almostEqual(res.getx(), expectedRes.getx(), 0.001f));
    assert(almostEqual(res.gety(), expectedRes.gety(), 0.001f));

    if(DEBUGLEVEL) std::cout << "Test grad3 passed!" << std::endl;
}

/**
 * @brief Tests the gradient descent algorithm (Test case 4).
 */
void testgrad4() {
    line l2({0, 0}, {1, 1});
    line l1({0, 3}, {1, -1});
    std::vector<line> lines = {l1, l2};
    point sp({0, 0});
    point res = gradientDescent(sp, lines);
    if(DEBUGLEVEL) std::cout << "Result: (" << res.getx() << "|" << res.gety() << ")" << std::endl;

    point expectedRes = point({1.5f, 1.5f});
    assert(almostEqual(res.getx(), expectedRes.getx(), 1e-5f));
    assert(almostEqual(res.gety(), expectedRes.gety(), 1e-5f));

    if(DEBUGLEVEL) std::cout << "Test grad4 passed!" << std::endl;
}

/**
 * @brief Main function to execute all test cases.
 * 
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return int Execution status.
 */
int main(int argc, char const *argv[]) {
    if(DEBUGLEVEL) std::cout << "start";
    try {
        testdistance1();
        testdistance2();
        testdistance3();
        testdistance4();
        testdistance5();
        testdistance6();
        testdistance7();
        testdistance8();
    } catch (const std::exception &e) {
        std::cerr << "error in distance algo" << e.what() << '\n';
    }

    try {
        testgrad1();
        testgrad2();
        testgrad3();
        testgrad4();
    } catch (const std::exception &e) {
        std::cerr << "error in lin prog algo" << e.what() << '\n';
    }

    return 0;
}
