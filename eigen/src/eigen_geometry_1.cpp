#include <iostream>

#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

int main(int argc, char **argv) {

  AngleAxisd t_V(M_PI/4, Vector3d(0,0,1));  // 旋转向量，绕Z轴旋转45度
  Matrix3d t_R = t_V.matrix();  // 旋转向量通过调用matrix()可转换成旋转矩阵。
  Quaterniond t_Q(t_V);  // 四元数可以通过旋转向量做初始化

  // 1. 使用旋转的角度和旋转轴向量来初始化角轴
  AngleAxisd V1(M_PI/4, Vector3d(0,0,1));
  cout << "Rotation vector1: " << endl << V1.matrix() << endl;

  cout << "----------------------" << endl;
  cout << "t_Q: " << endl << t_Q.coeffs() << endl << endl;
  cout << "----------------------" << endl;

/**
 * 当旋转轴是z轴时，即(0,0,1)，旋转矩阵如下：
 * [cos(th) -sin(th) 0 0]
 * [sin(th)  cos(th) 0 0]
 * [     0        0  1 0]
 * [     0        0  0 1]
 */


// ========================BEGIN [旋转向量]2==================================
  // 2. 使用旋转矩阵转旋转向量的3种方式。a. 调用函数fromRotationMatrix() b. 通过=赋值 c. 使用旋转矩阵来对旋转向量进行初始化
  //// 2.1 使用旋转向量的fromRotationMatrix()函数来对旋转向量赋值（注意此方法为旋转向量独有，四元素没有）
  AngleAxisd V2;
  V2.fromRotationMatrix(t_R);
  cout << "Rotation vector2: " << endl << V2.matrix() << endl;
  //// 2.2 直接使用旋转矩阵来对旋转向量赋值
  AngleAxisd V3;
  V3 = t_R;
  cout << "Rotation vector3: " << endl << V3.matrix() << endl; 
  //// 2.3 使用旋转矩阵来对旋转向量进行初始化
  AngleAxisd V4(t_R);
  cout << "Rotation vector4: " << endl << V4.matrix() << endl;
// ========================END [旋转向量]2==================================


// ========================BEGIN [旋转向量]3==================================
  // 3. 使用四元素来对旋转向量赋值的2种方式。a. 通过=赋值 b. 用四元素对旋转向量初始化
  // 3.1 直接使用四元素来对旋转向量赋值
  AngleAxisd V5;
  V5 = t_Q;
  cout << "Rotation vector5: " << endl << V5.matrix() << endl;
  // 3.2 使用四元素来对旋转向量进行初始化
  AngleAxisd V6(t_Q);
  cout << "Rotation vector6: " << endl << V6.matrix() << endl;

// 2020.08.09  11:50  这份代码我已理解至此。

  Eigen::Isometry3d T1 = Eigen::Isometry3d::Identity();  // 虽然称为3d，实质上是4-by-4矩阵
  // ----1.对各个元素赋值----
  T1(0,0) = 1.000000e+00, T1(0,1) = 1.197624e-11, T1(0,2) = 1.704639e-10, T1(0,3) = 3.214096e-14;
  T1(1,0) = 1.197625e-11, T1(1,1) = 1.000000e+00, T1(1,2) = 3.562503e-10, T1(1,3) = -1.998401e-15;
  T1(2,0) = 1.704639e-10, T1(2,1) = 3.562503e-10, T1(2,2) = 1.000000e+00, T1(2,3) = -4.041212e-14;
  T1(3,0) =            0, T1(3,1) =            0, T1(3,2) =            0, T1(3,3) =             1;
  cout << "T1 from element assign:\n" << T1.matrix() << endl;
// ========================END [旋转向量]3==================================

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// 对四元数赋值的3种方法

  // 1. 使用旋转轴向量（此向量为单位向量）和旋转角度来初始化四元素，即使用q=[cos(A/2), n_x*sin(A/2),n_y*sin(A/2),n_z*sin(A/2)]
  // 以(0,0,1)为旋转轴，旋转45度
  Quaterniond Q1(cos((M_PI/4) / 2), 0 * sin((M_PI/4) / 2), 0 * sin((M_PI/4) / 2), 1 * sin((M_PI/4)/2));
  // 第一种输出四元素的方式
  cout << "Quaternion1: " << endl << Q1.coeffs() << endl;
  // 第二种输出四元素的方式
  cout << "Qx: " << Q1.x() << endl;
  cout << "Qy: " << Q1.y() << endl;
  cout << "Qz: " << Q1.z() << endl;
  cout << "Qw: " << Q1.w() << endl;

  // 2. 使用旋转矩阵转四元素方式
  Quaterniond Q2;
  Q2 = t_R;
  cout << "Quaternion2: " << endl << Q2.coeffs() << endl;
  
  //// 2.2 使用旋转矩阵来对四元素进行初始化
  Quaterniond Q3(t_R);
  cout << "Quaternion3: " << endl << Q3.coeffs() << endl;
  
  // 3. 使用旋转向量对四元数来赋值
  // 3.1 直接使用旋转向量对四元数来赋值
  Quaterniond Q4;
  Q4 = t_V;
  cout << "Quaternion4: " << endl << Q4.coeffs() << endl;

  // 3.2 使用旋转向量来对四元数进行初始化
  Quaterniond Q5(t_V);
  cout << "Quaternion5: " << endl << Q5.coeffs() << endl;

  // ----2. 通过旋转矩阵和平移向量赋值----
  Matrix3d rotation_matrix1 = Matrix3d::Identity();
  rotation_matrix1 << 1.000000e+00, 1.197624e-11, 1.704639e-10,
                      1.197625e-11, 1.000000e+00, 3.562503e-10,
                      1.704639e-10, 3.562503e-10, 1.000000e+00;

  Vector3d t1;
  t1 << 3.214096e-14, -1.998401e-15, -4.041212e-14;

  T1 = Isometry3d::Identity();
  T1.rotate(rotation_matrix1);
  T1.pretranslate(t1);

  cout << "T1 from r,t:\n" << T1.matrix() << endl;


}