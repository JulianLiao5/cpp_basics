/*************************************************************************
	> File Name: src/eigenMatrix.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年04月22日 星期日 19时31分47秒
 ************************************************************************/


#include<iostream>

#include <Eigen/Core>
#include <Eigen/Dense>

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

    // 先来个错误的吧，在Eigen里，你不能混合两种不同类型的矩阵
    // Eigen::Matrix<double, 2, 1> result_1 = matrix_23 * v_3d;
    // cout << result_1 << endl;
    Eigen::Matrix<double, 2, 1> result_2 = matrix_23.cast<double>() * v_3d;
    cout << result_2 << endl;

    Eigen::Matrix<float, 2, 1> result_3 = matrix_23 * vd_3d;
    cout << result_3 << endl;

    // Transpose and inverse:
    Eigen::MatrixXd A(3, 2);
    A << 1, 2,
      2, 3,
      3, 4;
    Eigen::MatrixXd B = A.transpose();  // the transpose of A is a 2*3 matrix

    Eigen::MatrixXd C = B * A;
    Eigen::MatrixXd D = (B * A).inverse();


    cout << "B--[BEGIN]" << endl;
    cout << B << endl;
    cout << "B--[END]" << endl;

    cout << "C--[BEGIN]" << endl;
    cout << C << endl;
    cout << "C--[END]" << endl;

    cout << "D--[BEGIN]" << endl;
    cout << D << endl;
    cout << "D--[END]" << endl;

    Eigen::Quaterniond q(2, 0, 1, -3);
    cout << "This quaternion consists of a scalar " << q.w() << " and a vector " << endl << q.vec() << endl;

    q.normalize();
    cout << "To represent rotation, we need to normalize it such that its length is " << q.norm() << endl;

    Eigen::Vector3d v(1, 2, -1);
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

    Eigen::Vector3d euler = q.toRotationMatrix().eulerAngles(0, 1, 2);
    cout << "Euler from quaternion in roll, pitch, yaw: " << endl << euler << endl;



    return 0;
}




