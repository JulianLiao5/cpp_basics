#include <signal.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
// #include <memory>

#include <yaml-cpp/node/node.h>
#include <yaml-cpp/yaml.h>

#include <occupancy_status_grid_map.h>
#include <sonar_barrier_range_finder.h>
#include <vehicle/can_obj.h>

#include <glog/logging.h>

#ifdef INT
#undef INT
#endif

#ifdef UCHAR
#undef UCHAR
#endif

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

std::ofstream allSonarsFile;
YAML::Node user_config;
std::shared_ptr<PIAUTO::perception::SonarBarrierRangeFinder>
    mSonar_barrier_finder;
std::shared_ptr<OccupancyStatusGridMap> OGM = nullptr;
int count = 0;

void ReDrawMap(YAML::Node ui_config,
               std::shared_ptr<OccupancyStatusGridMap> OGM) {
  OGM->Reset();

  double cell_size = ui_config["occupancy_grid_map"]["cell_size"].as<double>();
  std::vector<int> grid_size = PIAUTO::util::getVecFromNode<int>(
      ui_config["occupancy_grid_map"]["grid_size"]);
  int height = grid_size[0];
  int width = grid_size[1];
  int half_height_in_meters = static_cast<int>(height * cell_size / 2.0);
  int half_width_in_meters = static_cast<int>(width * cell_size / 2.0);
  std::vector<double> lanes_size = PIAUTO::util::getVecFromNode<double>(
      ui_config["visualization"]["lanes_size"]);
  std::vector<double> marks_size = PIAUTO::util::getVecFromNode<double>(
      ui_config["visualization"]["marks_size"]);

  for (int i = -2; i < half_width_in_meters; i++) {
    OGM->DrawLineInMap(cv::Point2d(i, -half_width_in_meters),
                       cv::Point2d(i, half_width_in_meters),
                       CellStatus::UNKNOWN);
  }

  double specify_x =
      ui_config["visualization"]["specify_line_base_on_x_value"].as<double>();
  OGM->DrawLineInMap(cv::Point2d(specify_x, -half_width_in_meters),
                     cv::Point2d(specify_x, half_width_in_meters),
                     CellStatus::UNKNOWN);
  double specify_y =
      ui_config["visualization"]["specify_line_base_on_y_value"].as<double>();
  OGM->DrawLineInMap(cv::Point2d(-half_height_in_meters, specify_y),
                     cv::Point2d(half_height_in_meters, specify_y),
                     CellStatus::UNKNOWN);

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
  double cell_size = ui_config["occupancy_grid_map"]["cell_size"].as<double>();
  std::vector<int> grid_size = PIAUTO::util::getVecFromNode<int>(
      ui_config["occupancy_grid_map"]["grid_size"]);
  int height = grid_size[0];
  int width = grid_size[1];
  int half_height_in_meters = static_cast<int>(height * cell_size / 2.0);
  int half_width_in_meters = static_cast<int>(width * cell_size / 2.0);
  int half_height_in_pixels = static_cast<int>(height / 2.0);
  int half_width_in_pixels = static_cast<int>(width / 2.0);
  std::vector<int> viz_origin_offset = PIAUTO::util::getVecFromNode<int>(
      ui_config["visualization"]["origin_offset"]);
  std::vector<int> viz_step = PIAUTO::util::getVecFromNode<int>(
      ui_config["visualization"]["marks_step"]);
  double viz_text_in_line =
      ui_config["visualization"]["text_location_ratio_in_line"].as<double>();

  cv::QtFont font = cv::fontQt("Times");
  // [BEGIN x-axis in map coordinate]
  for (int i = -(half_height_in_meters - 1); i < half_height_in_meters; i++) {
    cv::addText(m, std::to_string(i),
                cv::Point(half_height_in_pixels + viz_origin_offset[0],
                          half_height_in_pixels - (i * viz_step[0]) +
                              viz_origin_offset[1]),
                font);
  }
  // [END x-axis in map coordinate]

  // [BEGIN y-axis in map coordinate]
  for (int i = -(half_width_in_meters - 2); i < (half_width_in_meters - 1);
       i++) {
    cv::addText(m, std::to_string(i),
                cv::Point(half_width_in_pixels - (i * viz_step[1]) +
                              viz_origin_offset[0],
                          half_height_in_pixels + viz_origin_offset[1]),
                font);
  }
  // [END y-axis in map coordinate]
}

std::mutex mutexSonarObjs;
std::map<int, PIAUTO::perception::Perception_Obstacles> mObs;

void SonarUpdate(const PIAUTO::perception::Perception_Obstacles *sonar_objs) {
  count++;
  LOG(INFO) << "** " << count << " ** "
            << "BEGIN!";
  mutexSonarObjs.lock();
  int sonar_id = -1;
  if (sonar_objs->obstacles.size() > 0) {
    sonar_id = sonar_objs->obstacles[0].sensor_id;
  }
  LOG(INFO) << "    sonar_id:    " << sonar_id;
  if (-1 != sonar_id) {
    if (mObs.end() != mObs.find(sonar_id)) {
      mObs[sonar_id] = *sonar_objs;
    } else {
      mObs.insert(std::pair<int, PIAUTO::perception::Perception_Obstacles>(
          sonar_id, *sonar_objs));
    }
  }
  LOG(INFO) << "** " << count << " ** "
            << "END!";
  mutexSonarObjs.unlock();
}

void exit_handler(int s) { printf("exit_hander!\n"); }

/**
 * sonar's coordinate: x: front, y: left, z: up
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
    std::cout << "Usage: " << argv[0] << " config_pod_sensors.yaml"
              << std::endl;
    return -1;
  }
  google::InstallFailureSignalHandler();
  // install SIGNAL handler
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = exit_handler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);

  YAML::Node sonars_config = YAML::LoadFile(argv[1]);
  YAML::Node ui_config = YAML::LoadFile(argv[2]);
  PIAUTO::chassis::CanObj *CO = PIAUTO::chassis::CanObj::GetCanObj();

  // sonar barrier parameters
  PIAUTO::perception::SonarBarrierRangeFinder::Parameters parameters(
      sonars_config, CO);

  // sonar barrier finder
  mSonar_barrier_finder =
      std::make_shared<PIAUTO::perception::SonarBarrierRangeFinder>(parameters);

  std::vector<int> sonarIds =
      PIAUTO::util::getVecFromNode<int>(sonars_config["sonarIds"]);
  double size_per_pixel =
      ui_config["occupancy_grid_map"]["cell_size"].as<double>();
  std::vector<int> grid_size = PIAUTO::util::getVecFromNode<int>(
      ui_config["occupancy_grid_map"]["grid_size"]);
  int height = grid_size[0];
  int width = grid_size[1];
  cv::Size map_size(height, width);
  std::shared_ptr<OccupancyStatusGridMap> OGM =
      std::make_shared<OccupancyStatusGridMap>(size_per_pixel, map_size);
  allSonarsFile.open("./all_sonars_data.txt", std::ios::out);

  boost::signals2::connection sonar_connect =
      mSonar_barrier_finder->SubscribeToSonar(&SonarUpdate);

  while (1) {
#if DEBUG
    std::thread::id main_id = std::this_thread::get_id();
    cout << "[" << __func__ << "] in thread(" << main_id << ")" << endl;
#endif

    ReDrawMap(ui_config, OGM);
    mutexSonarObjs.lock();
    for (int sonar_id : sonarIds) {
      if (mObs.end() != mObs.find(sonar_id)) {
        for (PIAUTO::perception::Perception_Obstacle obs_item :
             mObs.at(sonar_id).obstacles) {
          LOG(INFO) << obs_item << std::endl;
          allSonarsFile << obs_item;
          OGM->DrawRectInMap(
              Rect(obs_item.pose.x, obs_item.pose.y, 0.25, 0.07));
        }
      }
    }
    mutexSonarObjs.unlock();

    cv::Mat m = OGM->Visualize();
    cv::namedWindow("map", cv::WINDOW_NORMAL);
    AddTextToMap(ui_config, m);
    cv::imshow("map", m);
    if ('q' == cv::waitKey(20)) {
      // maybe you can call unsubscribe here
      mSonar_barrier_finder->UnSubscribeToSonar(1);
      break;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }  // end of 'while (1)'

  allSonarsFile.close();
  // Reserve some time(here is 1 second) to let chassis heart beat thread,
  // Receive data thread exit.
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  return 0;
}
