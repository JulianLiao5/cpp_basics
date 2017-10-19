#include <iostream>
#include <cmath>

#define PI 3.14159265

int main(int argc, char *argv[]) {

    std::cout << "****************test case 1**********" << std::endl;
    double tan_70d;
    tan_70d = tan((70 * PI)/180);
    printf("tan_70d: %f\n", tan_70d);

    std::cout << "****************test case 2(atan test)**********" << std::endl;
    double param, result;
    param = 1.0;
    result = atan(param) * 180 / PI;
    printf("The arc tangent of %f is %f\n", param, result);

    std::cout << "****************test case 3(atan2 test)**********" << std::endl;
    double deltaX = -10.0, deltaY = 10.0;
    double result2;
    result2 = atan2(deltaY, deltaX) * 180 / PI;
    printf("The arc tangent for (%f, %f) is %f\n", deltaX, deltaY, result2);

    deltaX = 1.0, deltaY = 2.747477;
    result2 = atan2(deltaY, deltaX) * 180 / PI;
    printf("The arc tangent for (%f, %f) is %f\n", deltaX, deltaY, result2);
}
