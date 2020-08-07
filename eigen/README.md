

# Eigen中常用的变量

typedef | 名称 | 形式
----|----|----
Eigen::Matrix3d | 旋转矩阵 | 3-by-3
Eigen::AngleAxisd | 旋转向量 | 3-by-1
Eigen::Quaterniond | 四元素 | 4-by-1
Eigen::Vector3d | 平移向量 | 3-by-1
Eigen::Isometry3d | 欧式变换矩阵 | 4-by-4


# Eigen中不同旋转方法之间的转换

![trans_among_rotations](imgs/Eigen_rotation_transformator.png "trans_among_rotations")

3D空间中的变换：


变换类型|说明
----|----
欧式变换 | 无