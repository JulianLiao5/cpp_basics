/*************************************************************************
	> File Name: test_opencv.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年10月20日 星期五 14时15分15秒
 ************************************************************************/

#include<iostream>

#include <opencv2/core/affine.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

Mat src;

Point2i VertexLeftTop(-1, -1);
Point2i VertexRightDown(-1, -1);

void onMouse(int Event, int x, int y, int flags, void* param);
void onMouse2(int Event, int x, int y, int flags, void* param);

int main(int argc, char *argv[]) {
  cv::Matx33d test_eye = cv::Matx33d::eye();
  cout << "test_eye: " << endl << test_eye << endl;

  cv::Affine3d parent_T_ref = cv::Affine3d::Identity();
  cout << "Rotation: " << std::endl << parent_T_ref.rotation() << endl;
  cout << "Translation: " << std::endl << parent_T_ref.translation() << endl;

  cv::Vec3d src_r0_dst(1, 0, 0);
  cv::Vec3d src_r1_dst(0, 1, 0);
  const cv::Vec3d &src_r2_dst = src_r0_dst.cross(src_r1_dst);
  cout << "src_r2_dst: " << endl << src_r2_dst << endl;

  src = imread("../res/lena30.jpg", IMREAD_UNCHANGED);
  namedWindow("image", WINDOW_NORMAL);
  setMouseCallback("image", onMouse, NULL);

    while(true) {
        if(VertexLeftTop.x == -1 && VertexRightDown.x == -1) {
          imshow("image", src);
        }
        if(VertexLeftTop.x != -1 && VertexRightDown.x != -1) {
          rectangle(src, Rect(VertexLeftTop, VertexRightDown), Scalar(255, 0, 0), 2);
          VertexLeftTop.x = -1;
          VertexLeftTop.y = -1;
          VertexRightDown.x = -1;
          VertexRightDown.y = -1;
          imshow("image", src);
        }
        if(waitKey(50) == 27) {
            break;
        }
    }
    return 0;
}

void onMouse(int Event, int x, int y, int flags, void* param) {
    if(Event == CV_EVENT_LBUTTONDOWN) {
      printf("1111111111111111\n");
      VertexLeftTop.x = x;
      VertexLeftTop.y = y; 
    }
    if (Event == CV_EVENT_LBUTTONUP) {
      printf("22222222222222222\n");
      VertexRightDown.x = x;
      VertexRightDown.y = y;
    }
}

void onMouse2(int Event, int x, int y, int flags, void* param) {
    if(Event == CV_EVENT_LBUTTONDOWN) {
      Point2i pt = Point2i(x, y);
      circle(src, pt, 2, Scalar(0, 255, 0), 10, 8, 0);
    }
}
