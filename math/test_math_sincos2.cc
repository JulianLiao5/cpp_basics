#include <iostream>
#include <cmath>

#define PI 3.14159265

#define test_range 5.56

int main(int argc, char *argv[]) {
    std::cout << "****************test case 1(atan2 test)**********" << std::endl;
    double result_0;
    result_0 = atan2(0.00839787, −0.04526625) * 180 / Math_PI;
    printf("line: %d, result_0 is %f\n", __LINE__, result_0);

/**    std::cout << "****************test case 2(atan test)**********" << std::endl;
    double param_1, result_1;
    param_1 = 2.13 / test_range;
    result_1 = atan(param_1) * 180 / PI;
    double range = test_range / cos (result_1 * PI / 180);
    printf("line: %d, the arc tangent of %f is %f, range: %f\n", __LINE__, param_1, result_1, range);
    std::cout << "****************test case 2(atan test)**********" << std::endl;
    double param_1, result_1;
    param_1 = 2.13 / test_range;
    result_1 = atan(param_1) * 180 / PI;
    double range = test_range / cos (result_1 * PI / 180);
    printf("line: %d, the arc tangent of %f is %f, range: %f\n", __LINE__, param_1, result_1, range);**/

    //double dbl_result_1 = 2.0 * result_1;
    //printf("line: %d, doubel angle is: %f\n", __LINE__, dbl_result_1);
    //double cos_1 = cos(result_1 * PI / 180);
    //double distance_1 = 10 / cos_1;
    //printf("The cos of %f is %lf, distance: %lf\n", result_1, cos_1, distance_1);
/***
    double param_2, result_2;
    param_2 = 0.115;
    result_2 = atan(param_2) * 180 / PI;
    printf("The arc tangent of %f is %f\n", param_2, result_2);
    double cos_2 = cos(result_2 * PI / 180);
    double distance_2 = 10 / cos_2;
    printf("The cos of %f is %lf, distance: %lf\n", result_2, cos_2, distance_2);

    double param_3, result_3;
    param_3 = 0.20;
    result_3 = atan(param_3) * 180 / PI;
    printf("The arc tangent of %f is %f\n", param_3, result_3);
    double cos_3 = cos(result_3 * PI / 180);
    double distance_3 = 10 / cos_3;
    printf("The cos of %f is %lf, distance: %lf\n", result_3, cos_3, distance_3);

    double param_4, result_4;
    param_4 = 0.67;
    result_4 = atan(param_4) * 180 / PI;
    printf("The arc tangent of %f is %f\n", param_4, result_4);
    double cos_4 = cos(result_4 * PI / 180);
    double distance_4 = 10 / cos_4;
    printf("The cos of %f is %lf, distance: %lf\n", result_4, cos_4, distance_4);
***/
/***    double cos_20 = cos(20.0 * PI / 180);
    double distance_2_0m_20 = 2.0 / cos_20;
    printf("The cos of %f is %lf, distance: %lf\n", 20.0, cos_20, distance_2_0m_20);
***/
/**    double cos_30 = cos(30.0 * PI / 180);
    double distance_1_5m_30 = 1.5 / cos_30;
    printf("The cos of %f is %lf, distance: %lf\n", 30.0, cos_30, distance_1_5m_30);

    double cos_45 = cos(45.0 * PI / 180);
    double distance_1_5m_45 = 1.5 / cos_45;
    printf("The cos of %f is %lf, distance: %lf\n", 45.0, cos_45, distance_1_5m_45);
**/
/***
    double test_10 = 10.0;
    double cos_10 = cos(test_10 * PI / 180);
    double distance_10 = 10 / cos_10;
    printf("The cos of %f is %lf, distance: %lf\n", test_10, cos_10, distance_10);

    double test_11 = 11.0;
    double cos_11 = cos(test_11 * PI / 180);
    double distance_11 = 10 / cos_11;
    printf("The cos of %f is %lf, distance: %lf\n", test_11, cos_11, distance_11);

    double test_12 = 12.0;
    double cos_12 = cos(test_12 * PI / 180);
    double distance_12 = 10 / cos_12;
    printf("The cos of %f is %lf, distance: %lf\n", test_12, cos_12, distance_12);

    double test_13 = 13.0;
    double cos_13 = cos(test_13 * PI / 180);
    double distance_13 = 10 / cos_13;
    printf("The cos of %f is %lf, distance: %lf\n", test_13, cos_13, distance_13);

    double test_14 = 14.0;
    double cos_14 = cos(test_14 * PI / 180);
    double distance_14 = 10 / cos_14;
    printf("The cos of %f is %lf, distance: %lf\n", test_14, cos_14, distance_14);

    double test_15 = 15.0;
    double cos_15 = cos(test_15 * PI / 180);
    double distance_15 = 10 / cos_15;
    printf("The cos of %f is %lf, distance: %lf\n", test_15, cos_15, distance_15);
***/
/***
    double test_46 = 46.0;
    double cos_46 = cos(test_46 * PI / 180);
    double distance_46 = 10 / cos_46;
    printf("The cos of %f is %lf, distance: %lf\n", test_46, cos_46, distance_46);

    double test_47 = 47.0;
    double cos_47 = cos(test_47 * PI / 180);
    double distance_47 = 10 / cos_47;
    printf("The cos of %f is %lf, distance: %lf\n", test_47, cos_47, distance_47);

    double test_48 = 48.0;
    double cos_48 = cos(test_48 * PI / 180);
    double distance_48 = 10 / cos_48;
    printf("The cos of %f is %lf, distance: %lf\n", test_48, cos_48, distance_48);

    double test_49 = 49.0;
    double cos_49 = cos(test_49 * PI / 180);
    double distance_49 = 10 / cos_49;
    printf("The cos of %f is %lf, distance: %lf\n", test_49, cos_49, distance_49);

    double test_50 = 50.0;
    double cos_50 = cos(test_50 * PI / 180);
    double distance_50 = 10 / cos_50;
    printf("The cos of %f is %lf, distance: %lf\n", test_50, cos_50, distance_50);

    double test_60 = 60.0;
    double cos_60 = cos(test_60 * PI / 180);
    double distance_60 = 10 / cos_60;
    printf("The cos of %f is %lf, distance: %lf\n", test_60, cos_60, distance_60);

    double test_63 = 63.0;
    double cos_63 = cos(test_63 * PI / 180);
    double distance_63 = 10 / cos_63;
    printf("The cos of %f is %lf, distance: %lf\n", test_63, cos_63, distance_63);

    double test_57 = 57.0;
    double cos_57 = cos(test_57 * PI / 180);
    double distance_57 = 5 / cos_57;
    printf("The cos of %f is %lf, distance: %lf\n", test_57, cos_57, distance_57);
***/
/**
    double test_20 = 20.0;
    double tan_20 = tan(test_20 * PI / 180);
    double distance_20_2 = 2.0 * tan_20;
    printf("The tangent of %f is %f, distance: %f\n", test_20, tan_20, distance_20_2);
    double distance_20_2_5 = 2.5 * tan_20;
    printf("The tangent of %f is %f, distance: %f\n", test_20, tan_20, distance_20_2_5);
**/
/***    double test_30 = 30.0;
    double tan_30 = tan(test_30 * PI / 180);
    double distance_30 = 10.0 * tan_30;
    printf("The tangent of %f is %f, distance: %f\n", test_30, tan_30, distance_30);
***/
/**    double distance_30_1_5 = 1.5 * tan_30;
    printf("The tangent of %f is %f, distance: %f\n", test_30, tan_30, distance_30_1_5);
    double distance_30_2_0 = 2.0 * tan_30;
    printf("The tangent of %f is %f, distance: %f\n", test_30, tan_30, distance_30_2_0);
    double distance_30_2_5 = 2.5 * tan_30;
    printf("The tangent of %f is %f, distance: %f\n", test_30, tan_30, distance_30_2_5);
**/
/***    double cos_result = cos(result * PI / 180);
    double distance_result = 10 / cos_result;
    printf("The cos of %f is %lf, distance: %lf\n", result, cos_result, distance_result);

    double cos_result_2 = cos(result_2 * PI / 180);
    double distance_result_2 = 10 / cos_result_2;
    printf("The cos of %f is %lf, distance: %lf\n", result_2, cos_result_2, distance_result_2);
***/

    return 0;
}
