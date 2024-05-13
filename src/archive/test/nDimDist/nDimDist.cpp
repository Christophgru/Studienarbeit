#include <iostream>
#include <vector>
#include <cmath>

double dotProduct(const std::vector<double>& v1, const std::vector<double>& v2) {
    double sum = 0.0;
    for (int i = 0; i < v1.size(); ++i) {
        sum += v1[i] * v2[i];
    }
    return sum;
}

std::vector<double> vectorSubtract(const std::vector<double>& v1, const std::vector<double>& v2) {
    std::vector<double> result(v1.size());
    for (int i = 0; i < v1.size(); ++i) {
        result[i] = v1[i] - v2[i];
    }
    return result;
}

std::vector<double> scalarMultiply(const std::vector<double>& v, double scalar) {
    std::vector<double> result(v.size());
    for (int i = 0; i < v.size(); ++i) {
        result[i] = v[i] * scalar;
    }
    return result;
}

double vectorNorm(const std::vector<double>& v) {
    return std::sqrt(dotProduct(v, v));
}

double pointToLineDistance(const std::vector<double>& A, const std::vector<double>& B, const std::vector<double>& P) {
    std::vector<double> AP = vectorSubtract(P, A);
    std::vector<double> AB = vectorSubtract(B, A);

    double t = dotProduct(AP, AB) / dotProduct(AB, AB);
    std::vector<double> projection = scalarMultiply(AB, t);
    std::vector<double> d = vectorSubtract(AP, projection);

    return vectorNorm(d);
}

int main() {
    std::vector<double> A = {1.0, 2.0, 3.0};  // Example point A
    std::vector<double> B = {5.0, 6.0, 8.0};  // Example point B
    std::vector<double> P = {1.0, 5.0, 2.0};  // Example point P

    std::cout << "Distance from point P to line AB is " << pointToLineDistance(A, B, P) << std::endl;

    return 0;
}