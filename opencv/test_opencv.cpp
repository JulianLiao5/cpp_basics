/*************************************************************************
	> File Name: test_opencv.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年10月20日 星期五 14时15分15秒
 ************************************************************************/

#include<iostream>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

Point2i VertexLeftTop(-1, -1);
Point2i VertexRightDown(-1, -1);

void onMouse(int Event, int x, int y, int flags, void* param);

int main(int argc, char *argv[]) {
  Mat src = imread("../res/lena30.jpg", IMREAD_UNCHANGED);
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
