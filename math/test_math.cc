#include <iostream>
#include <cmath>

#define PI 3.14159265

int main(int argc, char *argv[]) {

    std::cout << "****************test case 1(sin test)**********" << std::endl;
    double sin_270d;
    sin_270d = sin((270 * PI)/180);
    printf("sin_270d: %f\n", sin_270d);

    std::cout << "****************test case 1(cos test)**********" << std::endl;
    double cos_270d;
    cos_270d = cos((270 * PI)/180);
    printf("cos_270d: %f\n", cos_270d);

    std::cout << "****************test case 2(atan test)**********" << std::endl;
    double param, result;
    param = 1.0;
    result = atan(param) * 180 / PI;
    printf("The arc tangent of %f is %f\n", param, result);

    double param_2, result_2;
    param_2 = 1.0 / 1.5;
    result_2 = atan(param_2) * 180 / PI;
    printf("The arc tangent of %f is %f\n", param_2, result_2);

    double deg_3 = tan(result_2 * PI / 180);
    printf("The tangent of %f is %f\n", result_2, deg_3);

    std::cout << "****************test case 3(atan2 test)**********" << std::endl;
    double deltaX = 0.1, deltaY = 0.05;
    double result2;
    result2 = atan2(deltaY, deltaX);
    printf("The arc tangent for (%f, %f) is %f\n", deltaX, deltaY, result2);

    deltaX = 10.0, deltaY = -10.0;
    result2 = atan2(deltaY, deltaX) * 180 / PI;
    printf("The arc tangent for (%f, %f) is %f\n", deltaX, deltaY, result2);

    deltaX = -10.0, deltaY = 10.0;
    result2 = atan2(deltaY, deltaX) * 180 / PI;
    printf("The arc tangent for (%f, %f) is %f\n", deltaX, deltaY, result2);

    deltaX = 1.0, deltaY = 2.747477;
    result2 = atan2(deltaY, deltaX) * 180 / PI;
    printf("The arc tangent for (%f, %f) is %f\n", deltaX, deltaY, result2);

    std::cout << "****************test case 3(sqrt test)**********" << std::endl;
    double sqrt_param, sqrt_result;
    sqrt_param = 1024.0;
    sqrt_result = sqrt(sqrt_param);
    printf("sqrt(%f) = %f\n", sqrt_param, sqrt_result);

    sqrt_param = 9.0;
    sqrt_result = sqrt(sqrt_param);
    printf("sqrt(%f) = %f\n", sqrt_param, sqrt_result);

    sqrt_param = 16.0;
    sqrt_result = sqrt(sqrt_param);
    printf("sqrt(%f) = %f\n", sqrt_param, sqrt_result);
}
