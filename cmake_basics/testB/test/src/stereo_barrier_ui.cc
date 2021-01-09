
#include <signal.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <thread>
// #include <memory>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include <glog/logging.h>

#include <yaml-cpp/node/node.h>
#include <yaml-cpp/yaml.h>

#include <occupancy_status_grid_map.h>
#include <stereo_barrier_range_finder.h>

#ifdef INT
#undef INT
#endif

#ifdef UCHAR
#undef UCHAR
#endif



//std::ofstream allRadarsFile;

static bool isExit = false;

static void intSigintHandler(int sig) {
  isExit = true;
  std::cout << "++++++++++++++ exit captured!" << std::endl;
}

void DrawMap(YAML::Node ui_config,
             std::shared_ptr<OccupancyStatusGridMap> OGM) {
  OGM->Reset();

  double cell_size =
      ui_config["occupancy_grid_map"]["cell_size"].as<double>();
  std::vector<int> grid_size = PIAUTO::utils::getVecFromNode<int>(
      ui_config["occupancy_grid_map"]["grid_size"]);
  int height = grid_size[0];
  int width = grid_size[1];
  int half_height_in_meters = static_cast<int>(height * cell_size / 2.0);
  int half_width_in_meters = static_cast<int>(width * cell_size / 2.0);
  std::vector<double> lanes_size = PIAUTO::utils::getVecFromNode<double>(
      ui_config["visualization"]["lanes_size"]);
  std::vector<double> marks_size = PIAUTO::utils::getVecFromNode<double>(
      ui_config["visualization"]["marks_size"]);

  for (int i = -2; i < half_width_in_meters; i++) {
    OGM->DrawLineInMap(cv::Point2d(i, -half_width_in_meters),
                       cv::Point2d(i, half_width_in_meters),
                       CellStatus::UNKNOWN);
  }

  double specify_x =
      ui_config["visualization"]["specify_line_base_on_x_value"]
          .as<double>();
  OGM->DrawLineInMap(cv::Point2d(specify_x, -half_width_in_meters),
                     cv::Point2d(specify_x, half_width_in_meters),
                     CellStatus::UNKNOWN);
  double specify_y =
      ui_config["visualization"]["specify_line_base_on_y_value"]
          .as<double>();
  OGM->DrawLineInMap(cv::Point2d(-half_height_in_meters, specify_y),
                     cv::Point2d(half_height_in_meters, specify_y),
                     CellStatus::UNKNOWN);

  // represents -45deg, -60deg, -120deg, -135deg.
  std::vector<double> degrees_fix_y = {45.0, 60.0, 120.0, 135.0};
  for (int item_fix_y : degrees_fix_y) {
    OGM->DrawLineInMap(
        cv::Point2d(-half_width_in_meters / tan(item_fix_y * M_PI / 180),
                    -half_width_in_meters),
        cv::Point2d(half_width_in_meters / tan(item_fix_y * M_PI / 180),
                    half_width_in_meters),
        CellStatus::UNKNOWN);
  }

  // represents -0deg, -30deg, -150deg.
  std::vector<double> degrees_fix_x = {0.0, 30.0, 150.0};
  for (int item_fix_x : degrees_fix_x) {
    OGM->DrawLineInMap(
        cv::Point2d(-half_height_in_meters,
                    -half_height_in_meters * tan(item_fix_x * M_PI / 180)),
        cv::Point2d(half_height_in_meters,
                    half_height_in_meters * tan(item_fix_x * M_PI / 180)),
        CellStatus::UNKNOWN);
  }

  // Adds two lanes.
  for (int i = -(half_width_in_meters - 1); i <= (half_width_in_meters - 1);
       i += 2) {
    for (int j = -3; j <= 3; j += 1) {
      OGM->DrawRectInMap(Rect(i, j, lanes_size[0], lanes_size[1]),
                         CellStatus::UNKNOWN);
    }
  }

  // Adds measure marks.
  for (int i = -(half_width_in_meters - 1); i < half_width_in_meters; i++) {
    if (0 == i || 5 == i) {
      OGM->DrawRectInMap(Rect(i, 0, marks_size[0], marks_size[1] * 2),
                         CellStatus::UNKNOWN);
      OGM->DrawRectInMap(Rect(0, i, marks_size[0], marks_size[1] * 2),
                         CellStatus::UNKNOWN);
    } else {
      OGM->DrawRectInMap(Rect(i, 0, marks_size[0], marks_size[1]),
                         CellStatus::UNKNOWN);
      OGM->DrawRectInMap(Rect(0, i, marks_size[0], marks_size[1]),
                         CellStatus::UNKNOWN);
    }
  }
}

void AddTextToMap(YAML::Node ui_config, cv::Mat m) {
  double cell_size =
      ui_config["occupancy_grid_map"]["cell_size"].as<double>();
  std::vector<int> grid_size = PIAUTO::utils::getVecFromNode<int>(
      ui_config["occupancy_grid_map"]["grid_size"]);
  int height = grid_size[0];
  int width = grid_size[1];
  int half_height_in_meters = static_cast<int>(height * cell_size / 2.0);
  int half_width_in_meters = static_cast<int>(width * cell_size / 2.0);
  int half_height_in_pixels = static_cast<int>(height / 2.0);
  int half_width_in_pixels = static_cast<int>(width / 2.0);
  std::vector<int> viz_origin_offset = PIAUTO::utils::getVecFromNode<int>(
      ui_config["visualization"]["origin_offset"]);
  std::vector<int> viz_step = PIAUTO::utils::getVecFromNode<int>(
      ui_config["visualization"]["marks_step"]);
  double viz_text_in_line =
      ui_config["visualization"]["text_location_ratio_in_line"].as<double>();

//  cv::QtFont font = cv::fontQt("Times");
  int fontface = cv::FONT_HERSHEY_SIMPLEX;
  double scale = 1;
  int thickness = 2;

  // [BEGIN x-axis in map coordinate]
  for (int i = -(half_height_in_meters - 1); i < half_height_in_meters; i++) {
//    cv::addText(m, std::to_string(i),
//                cv::Point(half_height_in_pixels + viz_origin_offset[0],
//                          half_height_in_pixels - (i * viz_step[0]) +
//                              viz_origin_offset[1]),
//                font);
    cv::putText(m, std::to_string(i), cv::Point(half_height_in_pixels + viz_origin_offset[0],
                                                half_height_in_pixels - (i * viz_step[0]) +
                                                    viz_origin_offset[1]),
                fontface, scale, CV_RGB(0, 0, 0), thickness, 8);
  }
  // [END x-axis in map coordinate]

  // [BEGIN y-axis in map coordinate]
  for (int i = -(half_width_in_meters - 2); i < (half_width_in_meters - 1);
       i++) {
//    cv::addText(m, std::to_string(i),
//                cv::Point(half_width_in_pixels - (i * viz_step[1]) +
//                              viz_origin_offset[0],
//                          half_height_in_pixels + viz_origin_offset[1]),
//                font);
    cv::putText(m, std::to_string(i), cv::Point(half_width_in_pixels - (i * viz_step[1]) +
                                                    viz_origin_offset[0],
                                                half_height_in_pixels + viz_origin_offset[1]),
                fontface, scale, CV_RGB(0, 0, 0), thickness, 8);
  }
  // [END y-axis in map coordinate]

  // tan(30deg) = 0.577
  // tan(45deg) = 1
  // tan(60deg) = 1.732
  std::vector<int> degrees_in_max_x = {-45, -30, 30, 45};
  for (int deg_item_in_max_x : degrees_in_max_x) {
//    cv::addText(
//        m, std::to_string(deg_item_in_max_x) + "deg",
//        cv::Point(
//            static_cast<int>(half_width_in_pixels +
//                half_height_in_pixels * viz_text_in_line *
//                    tan(deg_item_in_max_x * M_PI / 180.0)),
//            static_cast<int>(half_height_in_pixels * (1 - viz_text_in_line))),
//        font);
    cv::putText(m, std::to_string(deg_item_in_max_x) + "deg",
                cv::Point(
                    static_cast<int>(half_width_in_pixels +
                        half_height_in_pixels * viz_text_in_line *
                            tan(deg_item_in_max_x * M_PI / 180.0)),
                    static_cast<int>(half_height_in_pixels * (1 - viz_text_in_line))),
                fontface, scale, CV_RGB(0, 0, 0), thickness, 8);
  }

  std::vector<int> degrees_in_min_x = {-150, 150};
  for (int deg_item_in_min_x : degrees_in_min_x) {
//    cv::addText(
//        m, std::to_string(deg_item_in_min_x) + "deg",
//        cv::Point(
//            static_cast<int>(half_width_in_pixels -
//                half_height_in_pixels * viz_text_in_line *
//                    tan(deg_item_in_min_x * M_PI / 180.0)),
//            static_cast<int>(half_height_in_pixels * (1 + viz_text_in_line))),
//        font);
    cv::putText(m, std::to_string(deg_item_in_min_x) + "deg",
                cv::Point(
                    static_cast<int>(half_width_in_pixels -
                        half_height_in_pixels * viz_text_in_line *
                            tan(deg_item_in_min_x * M_PI / 180.0)),
                    static_cast<int>(half_height_in_pixels * (1 + viz_text_in_line))),
                fontface, scale, CV_RGB(0, 0, 0), thickness, 8);
  }

  std::vector<int> degrees_in_min_y_max_y = {-135, -120, -60, 60, 120, 135};
  for (int deg_item_in_min_y_max_y : degrees_in_min_y_max_y) {
//    cv::addText(
//        m, std::to_string(deg_item_in_min_y_max_y) + "deg",
//        cv::Point(
//            static_cast<int>(half_width_in_pixels *
//                (1 - ((deg_item_in_min_y_max_y < 0) ? 1 : -1) *
//                    viz_text_in_line)),
//            static_cast<int>(
//                half_height_in_pixels -
//                    half_width_in_pixels * viz_text_in_line /
//                        tan(std::abs(deg_item_in_min_y_max_y) * M_PI / 180))),
//        font);
    cv::putText(m, std::to_string(deg_item_in_min_y_max_y) + "deg",
                cv::Point(
                    static_cast<int>(half_width_in_pixels *
                        (1 - ((deg_item_in_min_y_max_y < 0) ? 1 : -1) *
                            viz_text_in_line)),
                    static_cast<int>(
                        half_height_in_pixels -
                            half_width_in_pixels * viz_text_in_line /
                                tan(std::abs(deg_item_in_min_y_max_y) * M_PI / 180))),
                fontface, scale, CV_RGB(0, 0, 0), thickness, 8);
  }
}

void exit_handler(int s) {
  printf("exit_hander!\n");
}

/**
 * radar's coordinate: x: front, y: left, z: up
 *         clockwise is +, [0deg, 69deg]
 *         counter-clockwise is -, [-69deg, 0deg)
 * map's coordinate: x: vertical, up+, down-; y: horizontal, left+, right-
 *         clockwise is +, [0deg, 180deg)
 *         counter-clockwise is -, [-180deg, 0deg)
 * image's coordinate:
 *         origin: left-top
 *         x points right, from left to right, [0, width], x <-> width
 *         y points down, from up to down, [0, height], y <-> height
 */
int main(int argc, char *argv[]) {
  if (argc == 1 || (argc == 2 && strstr(argv[1], "-h"))) {
    std::cout << "Usage: " << argv[0] << "  path_to/config_car_sensors.yaml" << "  path_to/config_ui.yaml" << std::endl;
    return -1;
  }
  // google::ParseCommandLineFlags(&argc, &argv, false);
  google::InstallFailureSignalHandler();
  // install SIGNAL handler
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = exit_handler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);

  // system signal register
  signal(SIGINT, intSigintHandler);

  YAML::Node user_config = YAML::LoadFile(argv[1]);
  YAML::Node ui_config = YAML::LoadFile(argv[2]);

  PIAUTO::perception::StereoBarrierRangeFinder::Parameters parameters(
      user_config);

  PIAUTO::perception::StereoBarrierRangeFinder stereo_barrier_finder(parameters);

  PIAUTO::perception::Perception_Obstacles p_obstacles;

  double size_per_pixel =
      ui_config["occupancy_grid_map"]["cell_size"].as<double>();
  std::vector<int> grid_size = PIAUTO::utils::getVecFromNode<int>(
      ui_config["occupancy_grid_map"]["grid_size"]);
  int height = grid_size[0];
  int width = grid_size[1];
  cv::Size map_size(height, width);
  std::shared_ptr<OccupancyStatusGridMap> OGM =
      std::make_shared<OccupancyStatusGridMap>(size_per_pixel, map_size);
  //allRadarsFile.open("./all_radars_data.txt", std::ios::out);

  while (!isExit) {
#if DEBUG
    std::thread::id main_id = std::this_thread::get_id();
    cout << "[" << __func__ << "] in thread(" << main_id << ")" << endl;
#endif

    if (!p_obstacles.obstacles.empty()) {
      p_obstacles.obstacles.clear();
    }

    DrawMap(ui_config, OGM);
    static int count = 0;
    count++;
    LOG(INFO) << "** " << count << " ** "
              << "BEGIN!";
    int result = stereo_barrier_finder.GetObstacles(&p_obstacles);
    if (0 == result) {
      LOG(INFO) << "[Success] obstacles num: " << p_obstacles.obstacles.size()
                << std::endl;

      //std::vector<std::vector<cv::Point>> draw_hull(p_obstacles.obstacles.size());
      int k = 0;
      for (PIAUTO::perception::Perception_Obstacle obs_item :
          p_obstacles.obstacles) {
        std::vector<cv::Point2d> polygon(obs_item.obs_hull.size());
        for (int i = 0; i < obs_item.obs_hull.size(); i++) {
          cv::Point2d p;
//          p.x = int(obs_item.obs_hull[i].x * 480 / 30); // distance in pixels
//          p.y = int(obs_item.obs_hull[i].y * 211.5 / 15 + 211.5); // tangential in pixels
          p.x = obs_item.obs_hull[i].x; // distance in meters
          p.y = obs_item.obs_hull[i].y; // tangential in meters
          std::cout << p.x << " " << p.y << std::endl;
          polygon[i] = p;
        }

        // draw lines to show the rectangle
        if (polygon.size() > 1)
          for (int j = 1; j < polygon.size(); j++) {
            OGM->DrawLineInMap(polygon[j - 1], polygon[j],
                               CellStatus::OCCUPIED);
          }

        //draw_hull[k] = polygon;
        k++;
        std::cout << obs_item << std::endl;
      }
    }
    LOG(INFO) << "** " << count << " ** "
              << "END!";

    cv::Mat m = OGM->Visualize();
    cv::namedWindow("map", cv::WINDOW_NORMAL);
    AddTextToMap(ui_config, m);

    cv::imshow("map", m);
    if ('q' == cv::waitKey(20)) {
      break;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }  // end of 'while (1)'

  //allRadarsFile.close();

  // Reserve some time(here is 1 second) to let chassis heart beat thread,
  // Receive data thread exit.
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  return 0;
}
