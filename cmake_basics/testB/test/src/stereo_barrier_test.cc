#include <iostream>
#include <signal.h>
#include <chrono>
#include <thread>

#include <opencv2/opencv.hpp>

#include <yaml-cpp/node/node.h>
#include <yaml-cpp/yaml.h>

#include <glog/logging.h>

#include <stereo_barrier_range_finder.h>

static bool isExit = false;

static void intSigintHandler(int sig) {
  isExit = true;
  std::cout << "++++++++++++++ exit captured!" << std::endl;
}

int main(int argc, char *argv[]) {
  if (argc == 1 || (argc == 2 && strstr(argv[1], "-h"))) {
    std::cout << "Usage: " << argv[0] << " path_to/config_dragonfly.yaml" << std::endl;
    return -1;
  }
  // system signal register
  signal(SIGINT, intSigintHandler);

  // log setting
  google::InitGoogleLogging(argv[0]);

  // INFO(0), WARNING(1), ERROR(2), FATAL(3)
  // level bigger than xx will be written to log
  FLAGS_minloglevel = 0;
  // level bigger than xx will be shown in terminal
  google::SetStderrLogging(4);

  YAML::Node user_config = YAML::LoadFile(argv[1]);

  PIAUTO::perception::StereoBarrierRangeFinder::Parameters parameters(
      user_config);

  PIAUTO::perception::StereoBarrierRangeFinder stereo_barrier_finder(parameters);

  PIAUTO::perception::Perception_Obstacles p_obstacles;

  while(!isExit)
  {
    if (!p_obstacles.obstacles.empty()) {
      p_obstacles.obstacles.clear();
    }

    int result = stereo_barrier_finder.GetObstacles(&p_obstacles);

    cv::Mat drawing = cv::Mat::zeros(cv::Size(480, 432), CV_8UC3);

    if (0 == result && p_obstacles.obstacles.size() > 0) {
      std::cout << "************* BEGIN  **************" << std::endl;
      std::cout << "[Success] obstacles num: " << p_obstacles.obstacles.size()
                << std::endl;
      std::cout << "[sensor_id  obj_id  pose_x  pose_y  vel_x  vel_y  power]" << std::endl;
      std::vector<std::vector<cv::Point>> draw_hull(p_obstacles.obstacles.size());
      int k = 0;
      for (PIAUTO::perception::Perception_Obstacle obs_item :
          p_obstacles.obstacles) {

        if(obs_item.obs_hull.size() > 0)
        {
          std::vector<cv::Point> polygon(obs_item.obs_hull.size());
          for (int i = 0; i < obs_item.obs_hull.size(); i++) {
            cv::Point2d p;
            p.x = int(obs_item.obs_hull[i].x * 480 / 30);
            p.y = int(obs_item.obs_hull[i].y * 16.0 + 250);
            std::cout << p.x << " " << p.y << std::endl;
            polygon[i] = p;
          }

          draw_hull[k] = polygon;
          k++;
          std::cout << obs_item << std::endl;
        }
      }

      std::cout << draw_hull.size() << std::endl;
      for (int i = 0; i < draw_hull.size(); i++) {
	      cv::Scalar color = cv::Scalar(255, 0, 0); // blue - color for convex hull
        cv::drawContours(drawing, draw_hull, i, color, 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
      }

      std::cout << "************* STOP  **************" << std::endl;
      std::cout << "\n\n\n";
    }

    cv::imshow("Obstacle Hull", drawing);

    if ('q' == cv::waitKey(100)) {
      break;
    }
  }

  return 0;
}