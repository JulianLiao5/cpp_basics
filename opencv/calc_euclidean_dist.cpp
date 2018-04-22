/*************************************************************************
	> File Name: calc_euclidean_dist.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年02月28日 星期三 20时36分31秒
 ************************************************************************/

#include<iostream>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;


float euclideanDist(cv::Point2f& a, cv::Point2f& b) {
    cv::Point2f diff = a - b;
    return cv::sqrt(diff.x*diff.x + diff.y*diff.y);
}

double distanceBtwPoints(const cv::Point2f &a, const cv::Point2f &b) {
    double xDiff = a.x - b.x;
    double yDiff = a.y - b.y;
    return std::sqrt((xDiff * xDiff) + (yDiff * yDiff));
}

int main(int argc, char **argv) {
    cv::Point2f a(0.0, 0.0);
    cv::Point2f b(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());

    std::cout << "b(" << b.x << ", " << b.y << ")" << std::endl;

    std::cout << "a : " << a << " - b : " << b << std::endl << std::endl;

    std::cout << "euclideanDist : " << euclideanDist(a, b) << std::endl;
    std::cout << "distanceBtwPoints : " << distanceBtwPoints(a, b) << std::endl;
    std::cout << "cv::norm : " << cv::norm(a - b) << std::endl;

    cv::Point2f aa(10, 10);
    cv::Point2f bb(100, 100);
    std::cout << "cv::norm(aa, bb) = " << cv::norm(aa - bb) << std::endl;
}
