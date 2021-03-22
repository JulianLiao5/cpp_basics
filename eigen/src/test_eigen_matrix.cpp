/*************************************************************************
	> File Name: src/eigenMatrix.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年04月22日 星期日 19时31分47秒
 ************************************************************************/


#include<iomanip>
#include<iostream>

#include <Eigen/Core>
#include <Eigen/Dense>

using namespace Eigen;
using namespace std;



int main(int argc, char** argv) {
    Matrix<long double, 3, 3> T_utm_ant;  // 从天线局部坐标系转至utm绝对坐标系下
    long double heading = -2.1198245495947527495;
    T_utm_ant << cosf(heading), sinf(heading), 196425.05365922278725, -sinf(heading), cosf(heading), 2509221.0837410204113, 0, 0, 1;
    cout << "==T_utm_ant==\n" << setprecision(16) << T_utm_ant << "\n\n";
    Matrix<long double, 3, 3> T_ant_vehicle;    // 从车体坐标系转至天线局部坐标系
    T_ant_vehicle << 1, 0, -0.25, 0, 1, -0.525, 0, 0, 1;
    Matrix<long double, 3, 3> T_utm_vehicle;  // 从车体坐标系转至utm绝对坐标系下
    T_utm_vehicle = T_utm_ant * T_ant_vehicle;
    cout << "==T_utm_vehicle==\n" << setprecision(16) << T_utm_vehicle << "\n\n";

    Matrix<long double, 3, 1> pose_in_vehicle;
    pose_in_vehicle << 8.023555107116699503, -0.83875358104705810547, 1;
    Matrix<long double, 3, 1> pose_in_utm;
    pose_in_utm = T_utm_vehicle * pose_in_vehicle;
    cout << "==pose_in_utm==\n" << setprecision(16) << pose_in_utm << "\n\n";


    // Eigen中所有向量和矩阵都是Eigen::Matrix，它是一个模板类。它的前3个参数为：数据类型，行，列
    // 声明一个2 * 3的float矩阵
    Eigen::Matrix<float, 2, 3> matrix_23;

    // Eigen通过typedef提供了许多内置类型，不过底层仍是Eigen::Matrix
    // Eigen::Vector3d本质上是Eigen::Matrix<double, 3, 1>，即三维向量
    Eigen::Vector3d v_3d;
    Eigen::Matrix<float, 3, 1> vd_3d;

    // Eigen::Matrix3d 实质上是 Eigen::Matrix<double, 3, 3>
    Eigen::Matrix3d matrix_33 = Eigen::Matrix3d::Zero();  // 初始化为0

    // 如果不确定矩阵大小，还可以使用动态大小的矩阵
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> matrix_dynamic;
    // 更简单的形式
    Eigen::MatrixXd matrix_x;

    matrix_23 << 1, 2, 3, 4, 5, 6;
    cout << matrix_23 << endl;

    // 用()访问矩阵中元素
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            cout << matrix_23(i, j) << "\t";
        }
        cout << endl;
    }

    // 矩阵和矩阵相乘('*')
    v_3d << 3, 2, 1;
    vd_3d << 4, 5, 6;

    // [1    2    3]    [3]    [10]
    //                  [2]  =
    // [4    5    6]    [1]    [28]
    // 先来个错误的吧，在Eigen里，你不能混合两种不同类型的矩阵
    // Eigen::Matrix<double, 2, 1> result_1 = matrix_23 * v_3d;
    // cout << result_1 << endl;
    Eigen::Matrix<double, 2, 1> result_2 = matrix_23.cast<double>() * v_3d;
    cout << result_2 << endl;

    cout << "------------------" << endl;

    // [1    2    3]    [4]    [32]
    //                  [5]  =
    // [4    5    6]    [6]    [77]
    Eigen::Matrix<float, 2, 1> result_3 = matrix_23 * vd_3d;
    cout << result_3 << endl;

    // Transpose and inverse:
    Eigen::MatrixXd A(3, 2);
    A << 1, 2,
      2, 3,
      3, 4;
    Eigen::MatrixXd B = A.transpose();  // the transpose of A is a 2*3 matrix

    //             [1    2]     [1    2    3]     [ 5     8    11]
    // A * B =     [2    3]  *                 =  [ 8    13    18]
    //             [3    4]     [2    3    4]     [11    18    25]
    //----------------------------------------------------------------
    //             [1    2    3]     [1    2]     [14    20]
    // B * A =                    *  [2    3]  =
    //             [2    3    4]     [3    4]     [20    29]
    Eigen::MatrixXd C = B * A;
    Eigen::MatrixXd D = (B * A).inverse();
    Eigen::MatrixXd G = C.inverse();

    cout << "G--[BEGIN]" << endl;
    cout << G << endl;
    cout << "G--[END]" << endl;

    cout << "A--[BEGIN]" << endl;
    cout << A << endl;
    cout << "A--[END]" << endl;

    cout << "B--[BEGIN]" << endl;
    cout << B << endl;
    cout << "B--[END]" << endl;

    cout << "C--[BEGIN]" << endl;
    cout << C << endl;
    cout << "C--[END]" << endl;

    cout << "D--[BEGIN]" << endl;
    cout << D << endl;
    cout << "D--[END]" << endl;

    // ----------------BEGIN basic linear solving-----------------------
    // 当A2, b2, x2的type分别是Matrix3f, Vector3f, Vector3f时，结果不准确
    // The solution(x2) is:
    //       -2
    //        0.999997
    //        1
    // 当A2, b2, x2的type分别是Matrix3d, Vector3d, Vector3d时，结果准确
    // The solution(x2) is:
    //       -2
    //        1
    //        1
    //
    Matrix3d A2;
    Vector3d b2;
    A2 << 1,2,3,    4,5,6,    7,8,10;
    b2 << 3,3,4;
    cout << "A2:\n" << A2 << "\n\n";
    cout << "b2:\n" << b2 << "\n\n";
    Vector3d x2 = A2.colPivHouseholderQr().solve(b2);
    cout << "The solution(x2) is:\n" << x2 << endl;

    Matrix3d S8_T_veh_radar1;
    S8_T_veh_radar1 << 0.718126,  0.695913,  2.805000,
                        -0.695913,  0.718126, -0.730000,
                         0.000000,  0.000000,  1.000000;
    Vector3d S8_pose_in_veh;
    // S8_pose_in_veh << 3.20528, -1.1072, 1;
    S8_pose_in_veh << 2.90783, -1.09582, 1;
    Vector3d S8_pose_in_radar1 = S8_T_veh_radar1.colPivHouseholderQr().solve(S8_pose_in_veh);
    double S8_tmp_Azimuth = atan2(-S8_pose_in_radar1[1], S8_pose_in_radar1[0]) * 180 / M_PI;
    double S8_tmp_Range = S8_pose_in_radar1[0] / cos(S8_tmp_Azimuth * M_PI / 180);
    cout << "\nS8_tmp_Range: " << S8_tmp_Range << ", S8_tmp_Azimuth: " << S8_tmp_Azimuth << "\n";

    Matrix3d POD4_T_veh_radar0;
    POD4_T_veh_radar0 << 1.000000,  0.000000,  1.860000,
                         0.000000,  1.000000,  0.000000,
                         0.000000,  0.000000,  1.000000;
    Vector3d pod4_pose_in_veh;
    pod4_pose_in_veh << 4.80039, -1.91682, 1;
    Vector3d pod4_pose_in_radar0 = POD4_T_veh_radar0.colPivHouseholderQr().solve(pod4_pose_in_veh);
    double pod4_tmp_Azimuth = atan2(-pod4_pose_in_radar0[1], pod4_pose_in_radar0[0]) * 180 / M_PI;
    double pod4_tmp_Range = pod4_pose_in_radar0[0] / cos(pod4_tmp_Azimuth * M_PI / 180);
    cout << "\npod4_tmp_Range: " << pod4_tmp_Range << ", pod4_tmp_Azimuth: " << pod4_tmp_Azimuth << "\n";

    Matrix3d POD4_T_veh_radar1;
    POD4_T_veh_radar1 << 0.707107,  0.707107,  1.810000,
                        -0.707107,  0.707107, -0.687000,
                         0.000000,  0.000000,  1.000000;
    Matrix3d POD4_T_veh_radar5;
    POD4_T_veh_radar5 << 0.707107, -0.707107,  1.810000,
                         0.707107,  0.707107,  0.690000,
                         0.000000,  0.000000,  1.000000;
    Vector3d zzz_pose_in_veh;
    zzz_pose_in_veh << 5.46819,-0.572037,1;
    Vector3d zzz_pose_in_radar1 = POD4_T_veh_radar1.colPivHouseholderQr().solve(zzz_pose_in_veh);
    double zzz_tmp_Azimuth = atan2(-zzz_pose_in_radar1[1], zzz_pose_in_radar1[0]) * 180 / M_PI;
    double zzz_tmp_Range = zzz_pose_in_radar1[0] / cos(zzz_tmp_Azimuth * M_PI / 180);
    cout << "\n[POD4(20210109) | radar - 1] Range: " << zzz_tmp_Range << ", Azimuth: " << zzz_tmp_Azimuth << "\n";


    Vector3d yyy_pose_in_veh;
    yyy_pose_in_veh << 3.41475,1.02818,1;
    Vector3d yyy_pose_in_radar5 = POD4_T_veh_radar5.colPivHouseholderQr().solve(yyy_pose_in_veh);
    double yyy_tmp_Azimuth = atan2(-yyy_pose_in_radar5[1], yyy_pose_in_radar5[0]) * 180 / M_PI;
    double yyy_tmp_Range = yyy_pose_in_radar5[0] / cos(yyy_tmp_Azimuth * M_PI / 180);
    cout << "\n[POD4(20210109) | radar - 5] Range: " << setprecision(2) << fixed << yyy_tmp_Range << ", Azimuth: " << setprecision(2) << fixed << yyy_tmp_Azimuth << "\n";



    Vector3d S12_pose_in_veh;
    Vector3d S12_pose_in_radar;
    double S12_Azimuth = 0.0;
    double S12_Range = 0.0;

    Matrix3d S12_T_veh_radar0;
    S12_T_veh_radar0 << 1, 0,  3.186,
                        0,  1,  0,
                        0.000000,  0.000000,  1.000000;
    S12_pose_in_veh << 3.7201,-0.512186,1;
    S12_pose_in_radar = S12_T_veh_radar0.colPivHouseholderQr().solve(S12_pose_in_veh);
    S12_Azimuth = atan2(-S12_pose_in_radar[1], S12_pose_in_radar[0]) * 180 / M_PI;
    S12_Range = S12_pose_in_radar[0] / cos(S12_Azimuth * M_PI / 180);
    cout << "\n[S12(20210112) | radar - 0] Range: " << S12_Range << ", Azimuth: " << S12_Azimuth << "\n";

    Matrix3d S12_T_veh_radar2;
    S12_T_veh_radar2 << 0.829038, -0.559193,  2.98,
                        0.559193,  0.829038,  0.745,
                        0.000000,  0.000000,  1.000000;
    S12_pose_in_veh << 8.25842,0.874001,1;
    S12_pose_in_radar = S12_T_veh_radar2.colPivHouseholderQr().solve(S12_pose_in_veh);
    S12_Azimuth = atan2(-S12_pose_in_radar[1], S12_pose_in_radar[0]) * 180 / M_PI;
    S12_Range = S12_pose_in_radar[0] / cos(S12_Azimuth * M_PI / 180);
    cout << "\n[S12(20210111) | radar - 2] Range: " << S12_Range << ", Azimuth: " << S12_Azimuth << "\n";

    Matrix3d S12_T_veh_sonar1;
    S12_T_veh_sonar1 << 1.000000,  0.000000,  3.120000,
                        0.000000,  1.000000, -0.428000,
                        0.000000,  0.000000,  1.000000;
    Vector3d a_pose_in_veh;
    a_pose_in_veh << 7.24, -0.428, 1;
    Vector3d a_pose_in_sonar1 = S12_T_veh_sonar1.colPivHouseholderQr().solve(a_pose_in_veh);
    cout << "a | a_pose_in_sonar1: " << endl << a_pose_in_sonar1 << endl;


    Matrix3d S12_T_veh_sonar2;
    S12_T_veh_sonar2 << 0.000000,  1.000000,  1.805000,
                       -1.000000,  0.000000, -0.767000,
                        0.000000,  0.000000,  1.000000;
    Vector3d c_pose_in_veh;
    c_pose_in_veh << 1.805, -1.227, 1;
    Vector3d c_pose_in_sonar2 = S12_T_veh_sonar2.colPivHouseholderQr().solve(c_pose_in_veh);
    cout << "c | c_pose_in_sonar2: " << endl << c_pose_in_sonar2 << endl;


    Matrix3d S12_T_vehicle_radar0;
    S12_T_vehicle_radar0 << 1.000000,  0.000000,  3.186000,
                            0.000000,  1.000000,  0.000000,
                            0.000000,  0.000000,  1.000000;
    Vector3d aaa_pose_in_veh;
    aaa_pose_in_veh << 5.16196, 1.05507, 1;
    Vector3d aaa_pose_in_radar0 = S12_T_vehicle_radar0.colPivHouseholderQr().solve(aaa_pose_in_veh);
    double aaa_tmp_Azimuth = atan2(-aaa_pose_in_radar0[1], aaa_pose_in_radar0[0]) * 180 / M_PI;
    double aaa_tmp_Range = aaa_pose_in_radar0[0] / cos(aaa_tmp_Azimuth * M_PI / 180);
    cout << "\naaaaaaaaaaaaaa aaa_tmp_Range: " << aaa_tmp_Range << ", aaa_tmp_Azimuth: " << aaa_tmp_Azimuth << "\n";

    Matrix3d S12_T_vehicle_radar1;
    S12_T_vehicle_radar1 << 0.754710,  0.656059,  2.980000,
                           -0.656059,  0.754710, -0.745000,
                            0.000000,  0.000000,  1.000000;
    Vector3d tmp_pose_in_veh;
    tmp_pose_in_veh << 12.5334, -6.85466, 1;
    Vector3d pose_in_radar1 = S12_T_vehicle_radar1.colPivHouseholderQr().solve(tmp_pose_in_veh);
    double tmp_Azimuth = atan2(-pose_in_radar1[1], pose_in_radar1[0]) * 180 / M_PI;
    double tmp_Range = pose_in_radar1[0] / cos(tmp_Azimuth * M_PI / 180);
    cout << "Line" << __LINE__ << " | curcur -- tmp_Range: " << tmp_Range << ", tmp_Azimuth: " << tmp_Azimuth << " --curcur--\n";

    Matrix3d S12_T_vehicle_radar2;
    S12_T_vehicle_radar2 << 0.829038, -0.559193,  2.980000,
                            0.559193,  0.829038,  0.745000,
                            0.000000,  0.000000,  1.000000;
    Vector3d d_pose_in_veh;
    d_pose_in_veh << 4.23023, 2.59157, 1;
    Vector3d d_pose_in_radar2 = S12_T_vehicle_radar2.colPivHouseholderQr().solve(d_pose_in_veh);
    double d_Azimuth = atan2(-d_pose_in_radar2[1], d_pose_in_radar2[0]) * 180 / M_PI;
    double d_Range = d_pose_in_radar2[0] / cos(d_Azimuth * M_PI / 180);
    cout << "\ndddd -- d_Range: " << d_Range << ", d_Azimuth: " << d_Azimuth << " --dddd--\n";

    // Matrix3d S12_T_vehicle_radar2;
    // S12_T_vehicle_radar2 << 

    Matrix3d T_vehicle_radar2;
    Vector3d pose_in_vehicle2;
    T_vehicle_radar2 << 0, 1,  0.935,    -1, 0, -0.755,    0,0,1;
    pose_in_vehicle2 << 1.48406, -1.25112, 1;
    cout << "T_vehicle_radar2:\n" << T_vehicle_radar2 << "\n\n";
    cout << "pose_in_vehicle2:\n" << pose_in_vehicle2 << "\n\n";
    Vector3d pose_in_radar2 = T_vehicle_radar2.colPivHouseholderQr().solve(pose_in_vehicle2);
    cout << "The solution(pose_in_radar2) is:\n" << pose_in_radar2 << endl;

    double Azimuth = atan2(-pose_in_radar2[1], pose_in_radar2[0]) * 180 / M_PI;
    double Range = pose_in_radar2[0] / cos(Azimuth * M_PI / 180);
    cout << "\nRange: " << Range << ", Azimuth: " << Azimuth << "\n";

    // ----------------END basic linear solving-----------------------

    Eigen::Quaterniond q(2, 0, 1, -3);
    cout << "This quaternion consists of a scalar " << q.w() << " and a vector " << endl << q.vec() << endl;

    q.normalize();
    cout << "To represent rotation, we need to normalize it such that its length is " << q.norm() << endl;

    Eigen::Vector3d v(1, 2, -1);
    cout << "line: " << __LINE__ << "v.rows: " << v.rows() << ", v.cols: " << v.cols() << endl;
    Eigen::Quaterniond p;
    p.w() = 0;
    p.vec() = v;
    Eigen::Quaterniond rotatedP = q * p * q.inverse();
    Eigen::Vector3d rotatedV = rotatedP.vec();
    std::cout << "We can now use it to rotate a vector " << std::endl << v << " to " << std::endl << rotatedV << endl;

    Eigen::Matrix3d R = q.toRotationMatrix();  // convert a quaternion to a 3*3 rotation matrix
    cout << "R--[BEGIN]" << endl;
    cout << R << endl;
    cout << "R--[END]" << endl;
    cout << "Compare with the result using an rotation matrix " << endl << (R * v) << std::endl;

    cout << "the sum of each column of R: " << R.colwise().sum() << "\n\n";
    cout << "the maximum value of each column of R: " << R.colwise().maxCoeff() << "\n\n";
    cout << "the maximum absolute value of each column of R: " << R.cwiseAbs().colwise().maxCoeff() << "\n\n";

    Eigen::Vector3d euler = q.toRotationMatrix().eulerAngles(0, 1, 2);
    cout << "Euler from quaternion in roll, pitch, yaw: " << endl << euler << endl;



    return 0;
}
