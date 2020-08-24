#include <iostream>
#include <math.h>

#define PI 3.14159265

#define test_range 5.56

int main(int argc, char *argv[]) {
    std::cout << "****************test case 1(atan2 test)**********" << std::endl;
    double result_0 = 0.0;
    result_0 = atan2(0.046036246, 0.000474457) * 180 / M_PI;
    printf("line: %d, result_0 is %f\n", __LINE__, result_0);

    std::cout << "****************test case 1(asin test)**********" << std::endl;
    double res_asin = 0.0;
    res_asin = asin(-0.99893962) * 180 / M_PI;
    printf("line: %d, res_asin is %f\n", __LINE__, res_asin);

    return 0;
}
