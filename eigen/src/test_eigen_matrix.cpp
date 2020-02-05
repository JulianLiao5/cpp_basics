/*************************************************************************
	> File Name: src/eigenMatrix.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年04月22日 星期日 19时31分47秒
 ************************************************************************/


#include<iostream>

#include <Eigen/Core>
#include <Eigen/Dense>

using namespace Eigen;
using namespace std;



int main(int argc, char** argv) {
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

    Matrix3d T_vehicle_radar2;
    Vector3d pose_in_vehicle;
    T_vehicle_radar2 << 0, 1,  0.935,    -1, 0, -0.755,    0,0,1;
    pose_in_vehicle << 1.48406, -1.25112, 1;
    cout << "T_vehicle_radar2:\n" << T_vehicle_radar2 << "\n\n";
    cout << "pose_in_vehicle:\n" << pose_in_vehicle << "\n\n";
    Vector3d pose_in_radar2 = T_vehicle_radar2.colPivHouseholderQr().solve(pose_in_vehicle);
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
