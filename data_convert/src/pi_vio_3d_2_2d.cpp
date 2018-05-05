/*************************************************************************
	> File Name: pi_vio_3d_2_2d.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年04月23日 星期一 15时52分06秒
 ************************************************************************/

#include<iostream>
#include <fstream>
        // ofstream

#include <string>
        // to_string

#include <stdio.h>
        // fopen | fgets | feof | sscanf

#include <Eigen/Core>
#include <Eigen/Dense>

using namespace std;


ofstream pose_2d_file;

double NormalizeAngle(const double angle) {
  double normalized_angle = angle;
    while (normalized_angle < -M_PI) {
      normalized_angle += 2 * M_PI;
    }
    while (normalized_angle > M_PI) {
      normalized_angle -= 2 * M_PI;
    }
    return normalized_angle;
}


// for PIVIO and msckf, the rule is different.
int main(int argc, char ** argv) {
    if (argc < 2) {
      cout << "Usage: ./converter_pi_3d_2_2d pose_3d.txt" << endl;
      return -1;
    }

    FILE *pose_3d_fp;
    char buffer[1024];
    if ((pose_3d_fp = fopen(argv[1], "r")) == NULL) {
        cout << "read file " << argv[1] << " error!" << endl;
        return -2;
    }

    pose_2d_file.open("./pose_2d_vio.csv", ios::out);
    while (!feof(pose_3d_fp)) {
        fgets(buffer, 1024, pose_3d_fp);

/*        double value[8] = {0};

        sscanf(buffer, "%lf %lf %lf %lf %lf %lf %lf %lf", &value[0], &value[1], &value[2], &value[3],
              &value[4], &value[5], &value[6], &value[7]);

        Eigen::Quaterniond q(value[7], value[4], value[5], value[6]);
        Eigen::Vector3d euler = q.toRotationMatrix().eulerAngles(0, 1, 2);

        // for msckf
        pose_2d_file << std::to_string(value[0]) << " " << value[2] << " " << ((-1.0) * (value[1]))
            << " " << NormalizeAngle((euler(2))) << endl;*/

        double value[3] = {0};

        sscanf(buffer, "%lf,%lf,%lf", &value[0], &value[1], &value[2]);

        // for msckf
        pose_2d_file << std::to_string(value[0]) << " " << value[2] << " " << ((-1.0) * (value[1])) << endl;

        // for PIVIO
        //pose_2d_file << std::to_string(value[0]) << " " << value[3] << " " << ((-1.0) * (value[1]))
        //    << " " << NormalizeAngle((euler(1) + M_PI)) << endl;
    }
    pose_2d_file.close();

    return 0;
}
