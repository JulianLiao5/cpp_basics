

#include <cmath>
#include <iostream>


int main(int argc, char *argv[]) {
    const double squareDistFromObs = pow(0.3, 2) + pow(0.45, 2);
    const double squareSigmaObs = pow(0.1, 2);
    double exponentialVal = exp(- squareDistFromObs / squareSigmaObs);
    std::cout << "squareDistFromObs: " << squareDistFromObs << std::endl
        << ",  squareSigmaObs: " << squareSigmaObs << std::endl
        << "        exponentialVal: " << exponentialVal << std::endl;

    return 0;
}
