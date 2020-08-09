

# Eigen中常用的变量

typedef | 名称 | 形式
----|----|----
Eigen::Matrix3d | 旋转矩阵 | 3-by-3
Eigen::AngleAxisd | 旋转向量 | 3-by-1
Eigen::Quaterniond | 四元素 | 4-by-1
Eigen::Vector3d | 平移向量 | 3-by-1
Eigen::Isometry3d | 欧式变换矩阵 | 4-by-4

## AngleAxis

Angle + Axis，Angle代表旋转角，Axis代表旋转轴，用绕任意的旋转轴旋转一个角度来表示3D旋转。

### 构造函数

形式1: Eigen::AngleAxis::AngleAxis(const Scalar &angle, const MatrixBase<Derived> &axis)

注意：
- angle是in radian，例如 M_PI/4
- axis必须是归一化的，Vector3d(0, 0, 1)，这就是归一化的

## 四元数(Quaternion)

与旋转矩阵类似，我们仍然假设某个旋转是绕单位向量![unit rotation vector](imgs/unit_vector.gif "unit rotation vector")进行了角度为th的旋转，那么这个旋转的四元数形式为：

![quaternion](imgs/quaternion_from_rotation_vector.gif "quaternion1")

旋转轴为(0,0,1)，旋转角度th为45度，那么对应的四元数的表示形式为：

nx * sin(22.5deg) = 0

ny * sin(22.5deg) = 0

nz * sin(22.5deg) = 0.382683

cos(22.5deg) = 0.923880

sqrt(0.382683) + sqrt(0.923880) = 1

想想直角三角形，sqrt(sin(th)) + sqrt(cos(th)) = 1


# Eigen中不同旋转方法之间的转换

![trans_among_rotations](imgs/Eigen_rotation_transformator.png "trans_among_rotations")

3D空间中的变换：


变换类型|说明
----|----
欧式变换 | 无