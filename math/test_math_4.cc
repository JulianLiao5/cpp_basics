#include <iostream>
#include <cmath>

#define PI 3.14159265

int main(int argc, char *argv[]) {
/***    std::cout << "****************test case 1(sin test)**********" << std::endl;
    double sin_270d;
    sin_270d = sin((270 * PI)/180);
    printf("sin_270d: %f\n", sin_270d);

    std::cout << "****************test case 1(cos test)**********" << std::endl;
    double cos_270d;
    cos_270d = cos((270 * PI)/180);
    printf("cos_270d: %f\n", cos_270d);   ***/

    /***
    std::cout << "****************test case 2(atan test)**********" << std::endl;
    double param, result;
    param = 0.45 / 15;
    result = atan(param) * 180 / PI;
    printf("The arc tangent of %f is %f\n", param, result);

    double param_2, result_2;
    param_2 = 1.25 / 15;
    result_2 = atan(param_2) * 180 / PI;
    printf("The arc tangent of %f is %f\n", param_2, result_2);

    double deg_3 = tan(result_2 * PI / 180);
    printf("The tangent of %f is %f\n", result_2, deg_3);    ***/
    double test_18_deg = 18.0;
    double cos_18 = cos(test_18_deg * PI / 180);
    double distance_18deg = 7 / cos_18;
    printf("The cos of %f is %lf, distance: %lf\n", test_18_deg, cos_18, distance_18deg);

    double test_19_deg = 19.0;
    double cos_19 = cos(test_19_deg * PI / 180);
    double distance_19deg = 7 / cos_19;
    printf("The cos of %f is %lf, distance: %lf\n", test_19_deg, cos_19, distance_19deg);
    double test_20_deg = 20.0;
    double cos_20 = cos(test_20_deg * PI / 180);
    double distance_20deg = 7 / cos_20;
    printf("The cos of %f is %lf, distance: %lf\n", test_20_deg, cos_20, distance_20deg);

    double test_21_deg = 21.0;
    double cos_21 = cos(test_21_deg * PI / 180);
    double distance_21deg = 7 / cos_21;
    printf("The cos of %f is %lf, distance: %lf\n", test_21_deg, cos_21, distance_21deg);

    double test_22_deg = 22.0;
    double cos_22 = cos(test_22_deg * PI / 180);
    double distance_22deg = 7 / cos_22;
    printf("The cos of %f is %lf, distance: %lf\n", test_22_deg, cos_22, distance_22deg);

    double test_25_deg = 25.0;
    double cos_25 = cos(test_25_deg * PI / 180);
    double distance_25deg = 7 / cos_25;
    printf("The cos of %f is %lf, distance: %lf\n", test_25_deg, cos_25, distance_25deg);

    double test_26_deg = 26.0;
    double cos_26 = cos(test_26_deg * PI / 180);
    double distance_26deg = 7 / cos_26;
    printf("The cos of %f is %lf, distance: %lf\n", test_26_deg, cos_26, distance_26deg);

    double test_27_deg = 27.0;
    double cos_27 = cos(test_27_deg * PI / 180);
    double distance_27deg = 7 / cos_27;
    printf("The cos of %f is %lf, distance: %lf\n", test_27_deg, cos_27, distance_27deg);

    double test_28_deg = 28.0;
    double cos_28 = cos(test_28_deg * PI / 180);
    double distance_28deg = 7 / cos_28;
    printf("The cos of %f is %lf, distance: %lf\n", test_28_deg, cos_28, distance_28deg);

    double test_29_deg = 29.0;
    double cos_29 = cos(test_29_deg * PI / 180);
    double distance_29deg = 7 / cos_29;
    printf("The cos of %f is %lf, distance: %lf\n", test_29_deg, cos_29, distance_29deg);
/***
    double test_32_deg = 32.0;
    double cos_32 = cos(test_32_deg * PI / 180);
    double distance_32deg = 10 / cos_32;
    printf("The cos of %f is %lf, distance: %lf\n", test_32_deg, cos_32, distance_32deg);

    double test_33_deg = 33.0;
    double cos_33 = cos(test_33_deg * PI / 180);
    double distance_33deg = 10 / cos_33;
    printf("The cos of %f is %lf, distance: %lf\n", test_33_deg, cos_33, distance_33deg);

    double test_34_deg = 34.0;
    double cos_34 = cos(test_34_deg * PI / 180);
    double distance_34deg = 10 / cos_34;
    printf("The cos of %f is %lf, distance: %lf\n", test_34_deg, cos_34, distance_34deg);

    double test_35_deg = 35.0;
    double cos_35 = cos(test_35_deg * PI / 180);
    double distance_35deg = 10 / cos_35;
    printf("The cos of %f is %lf, distance: %lf\n", test_35_deg, cos_35, distance_35deg);
    double test_16_deg = 16.0;
    double cos_16 = cos(test_16_deg * PI / 180);
    double distance_16deg = 10 / cos_16;
    printf("The cos of %f is %lf, distance: %lf\n", test_16_deg, cos_16, distance_16deg);

    double test_17_deg = 17.0;
    double cos_17 = cos(test_17_deg * PI / 180);
    double distance_17deg = 10 / cos_17;
    printf("The cos of %f is %lf, distance: %lf\n", test_27_deg, cos_27, distance_27deg);

    double test_28_deg = 28.0;
    double cos_28 = cos(test_28_deg * PI / 180);
    double distance_28deg = 10 / cos_28;
    printf("The cos of %f is %lf, distance: %lf\n", test_28_deg, cos_28, distance_28deg);

    double test_29_deg = 29.0;
    double cos_29 = cos(test_29_deg * PI / 180);
    double distance_29deg = 10 / cos_29;
    printf("The cos of %f is %lf, distance: %lf\n", test_29_deg, cos_29, distance_29deg);

    double test_40_deg = 40.0;
    double cos_40 = cos(test_40_deg * PI / 180);
    double distance_40deg = 10 / cos_40;
    printf("The cos of %f is %lf, distance: %lf\n", test_40_deg, cos_40, distance_40deg);
***/
    /***
    double test_45_deg = 45.0;
    double cos_45 = cos(test_45_deg * PI / 180);
    double distance_45deg = 10 / cos_45;
    printf("line: %d, The cos of %f is %lf, distance: %lf\n", __LINE__, test_45_deg, cos_45, distance_45deg);

    cos_45 = cos(test_45_deg * PI / 180);
    distance_45deg = 10 / cos_45;
    printf("line: %d, The tangent of %f is %lf, distance: %lf\n", __LINE__, test_45_deg, cos_45, distance_45deg);
***/
/***    double test_20_deg = 20.0;
    double tan_20 = tan(test_20_deg * PI / 180);
    double distance_20deg_5 = 5 * tan_20;
    printf("line: %d, The tangent of %f is %lf, distance: %lf\n", __LINE__, test_20_deg, tan_20, distance_20deg_5);
    double distance_20deg_10 = 10 * tan_20;
    printf("line: %d, The tangent of %f is %lf, distance: %lf\n", __LINE__, test_20_deg, tan_20, distance_20deg_10);
    double distance_20deg_15 = 15 * tan_20;
    printf("line: %d, The tangent of %f is %lf, distance: %lf\n", __LINE__, test_20_deg, tan_20, distance_20deg_15);  ***/
/***
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
***/
    return 0;
}
