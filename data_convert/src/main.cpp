#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include <fstream>
#include "utm.h"

using namespace std;
using namespace cv;

#define DEG2RAD(x) ((x / 180.0) * M_PI)

// ofstream utmFile;
ofstream fix_coorFile;
ofstream float_coorFile;
ofstream gps_2d_File;

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

int main(int argc, char *argv[])
{
  Mat image(1000,1000,CV_8UC3, Scalar::all(255));
  Mat g_image(1000,1000,CV_8UC3, Scalar::all(255));
  UTMCoor utm;

  FILE *fp;
  char buffer[100];
  if((fp = fopen(argv[1],"r")) == NULL)
	std::cout << "error read" <<std::endl;

  double x = 0;
  double y = 0;
  int count = 0;

  double OFFSETX;
  double OFFSETY;

  // utmFile.open("./utm_data.txt", ios::out);
  fix_coorFile.open("./fix_coor.txt", ios::out);
  float_coorFile.open("./float_coor.txt", ios::out);
  gps_2d_File.open("./gps_pose_2d.csv", ios::out);

  // tow(sec),latitude(degrees),longitude(degrees),flags,heading_data(degree),heading_mag(degree)
  while (!feof(fp)) {
    fgets(buffer, 100, fp);
  
    double value[6] = {0};
  
    sscanf(buffer, "%lf,%lf,%lf,%lf,%lf,%lf", &value[0],&value[1],&value[2],&value[3],&value[4],&value[5]);
  
    double lat = value[1];
    double lon = value[2];
    LatLonToUTMXY(lat, lon, 0, &utm);
    // utmFile << utm.x << " " << utm.y << endl;
  
    static int flag =1;
    if(1 == flag) {
       OFFSETX = utm.x;
       OFFSETY = utm.y;
       printf("**************OFFSETX: %lf, OFFSETY: %lf\n", OFFSETX, OFFSETY);
       flag = 0;
    }
  
    x = utm.x - OFFSETX;
    y = utm.y - OFFSETY;

    Point points;
//    points.x = (x + 600);
//    points.y = (-y + 500);
    points.x = (x + 300);
    points.y = (-y + 300);

    if(4 == value[3]) {
      //if (std::abs(x) < 20000.0 && std::abs(y) < 20000.0) {
        gps_2d_File << std::to_string(value[0]) << " " << x << " " << y << " " << DEG2RAD(value[4]) << " " << DEG2RAD(value[5])
            << " " << value[3] << endl;
        fix_coorFile << x << "  " << y << std::endl;
        circle(image, points, 1, Scalar(255,0,0));
      // }
    } else if(3 == value[3]) {
      //if (std::abs(x) < 20000.0 && std::abs(y) < 20000.0) {
      gps_2d_File << std::to_string(value[0]) << " " << x << " " << y << " " << DEG2RAD(value[4]) << " " << DEG2RAD(value[5])
           << " " << value[3] << endl;
      float_coorFile << x << "  " << y << std::endl;
      circle(image, points, 1, Scalar(0,0,255));
      //}
    } else if(0 == value[3]) {
      static int num = 0;
      num ++;
      printf("zero %d \n", num);
    }
    count ++;
    if (count % 60 == 0 ) {
      imshow(" ",image);
      waitKey(1);
    }
    if (count > 1000000){
      image = g_image.clone();
      count = 0 ;
    }
  } // end 'while (!feof(fp))'

  // utmFile.close();
  fix_coorFile.close();
  float_coorFile.close();
  gps_2d_File.close();

  cout << "\nthe end" << endl;
  imwrite("image.png", image);
  return 0;
}
