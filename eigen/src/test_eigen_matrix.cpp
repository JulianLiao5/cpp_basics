/*************************************************************************
	> File Name: src/eigenMatrix.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年04月22日 星期日 19时31分47秒
 ************************************************************************/


#include<iostream>

#include <Eigen/Core>

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



}




