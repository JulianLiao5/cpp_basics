#include <signal.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
// #include <memory>

#include <yaml-cpp/node/node.h>
#include <yaml-cpp/yaml.h>

#include <occupancy_status_grid_map.h>
#include <radar_barrier_range_finder.h>
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

std::ofstream allRadarsFile;
std::shared_ptr<PIAUTO::perception::RadarBarrierRangeFinder>
    mRadar_barrier_finder;
std::shared_ptr<OccupancyStatusGridMap> OGM = nullptr;
int count = 0;

void DrawMap(YAML::Node ui_config,
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

  // tan(30deg) = 0.577
  // tan(45deg) = 1
  // tan(60deg) = 1.732
  std::vector<int> degrees_in_max_x = {-45, -30, 30, 45};
  for (int deg_item_in_max_x : degrees_in_max_x) {
    cv::addText(
        m, std::to_string(deg_item_in_max_x) + "deg",
        cv::Point(
            static_cast<int>(half_width_in_pixels +
                             half_height_in_pixels * viz_text_in_line *
                                 tan(deg_item_in_max_x * M_PI / 180.0)),
            static_cast<int>(half_height_in_pixels * (1 - viz_text_in_line))),
        font);
  }

  std::vector<int> degrees_in_min_x = {-150, 150};
  for (int deg_item_in_min_x : degrees_in_min_x) {
    cv::addText(
        m, std::to_string(deg_item_in_min_x) + "deg",
        cv::Point(
            static_cast<int>(half_width_in_pixels -
                             half_height_in_pixels * viz_text_in_line *
                                 tan(deg_item_in_min_x * M_PI / 180.0)),
            static_cast<int>(half_height_in_pixels * (1 + viz_text_in_line))),
        font);
  }

  std::vector<int> degrees_in_min_y_max_y = {-135, -120, -60, 60, 120, 135};
  for (int deg_item_in_min_y_max_y : degrees_in_min_y_max_y) {
    cv::addText(
        m, std::to_string(deg_item_in_min_y_max_y) + "deg",
        cv::Point(
            static_cast<int>(half_width_in_pixels *
                             (1 - ((deg_item_in_min_y_max_y < 0) ? 1 : -1) *
                                      viz_text_in_line)),
            static_cast<int>(
                half_height_in_pixels -
                half_width_in_pixels * viz_text_in_line /
                    tan(std::abs(deg_item_in_min_y_max_y) * M_PI / 180))),
        font);
  }
}

std::map<int, PIAUTO::perception::Perception_Obstacles> mObs;

void RadarUpdate(const PIAUTO::perception::Perception_Obstacles *radar_objs) {
  count++;
  LOG(INFO) << "** " << count << " ** "
            << "BEGIN!";
  int radar_id = -1;
  if (radar_objs->obstacles.size() > 0) {
    radar_id = radar_objs->obstacles[0].sensor_id;
  }
  LOG(INFO) << "    radar_id:    " << radar_id;
  if (-1 != radar_id) {
    if (mObs.end() != mObs.find(radar_id)) {
      mObs[radar_id] = *radar_objs;
    } else {
      mObs.insert(std::pair<int, PIAUTO::perception::Perception_Obstacles>(
          radar_id, *radar_objs));
    }
  }
  LOG(INFO) << "** " << count << " ** "
            << "END!";
}

void exit_handler(int s) { printf("exit_hander!\n"); }

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
  if (argc < 3) {
    std::cout << "Usage: " << argv[0]
              << " config_pod_sensors.yaml config_ui.yaml" << std::endl;
    return -1;
  }
  google::InstallFailureSignalHandler();
  // install SIGNAL handler
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = exit_handler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);

  YAML::Node radars_config = YAML::LoadFile(argv[1]);
  YAML::Node ui_config = YAML::LoadFile(argv[2]);
  PIAUTO::chassis::CanObj *CO = PIAUTO::chassis::CanObj::GetCanObj();

  // radar barrier parameters
  PIAUTO::perception::RadarBarrierRangeFinder::Parameters parameters(
      radars_config, CO);

  // radar barrier finder
  mRadar_barrier_finder =
      std::make_shared<PIAUTO::perception::RadarBarrierRangeFinder>(parameters);

  std::vector<int> radarIds =
      PIAUTO::util::getVecFromNode<int>(radars_config["radarIds"]);
  double size_per_pixel =
      ui_config["occupancy_grid_map"]["cell_size"].as<double>();
  std::vector<int> grid_size = PIAUTO::util::getVecFromNode<int>(
      ui_config["occupancy_grid_map"]["grid_size"]);
  int height = grid_size[0];
  int width = grid_size[1];
  cv::Size map_size(height, width);
  OGM = std::make_shared<OccupancyStatusGridMap>(size_per_pixel, map_size);
  allRadarsFile.open("./all_radars_data.txt", std::ios::out);

  boost::signals2::connection radar_connect =
      mRadar_barrier_finder->SubscribeToRadar(&RadarUpdate);

  while (1) {
    DrawMap(ui_config, OGM);
    for (int radar_id : radarIds) {
      if (mObs.end() != mObs.find(radar_id)) {
        for (PIAUTO::perception::Perception_Obstacle obs_item :
             mObs.at(radar_id).obstacles) {
          LOG(INFO) << obs_item << std::endl;
          allRadarsFile << obs_item;
          OGM->DrawRectInMap(
              Rect(obs_item.pose.x, obs_item.pose.y, 0.25, 0.07));
        }
      }
    }
    cv::Mat m = OGM->Visualize();
    cv::namedWindow("map", cv::WINDOW_NORMAL);
    AddTextToMap(ui_config, m);
    cv::imshow("map", m);
    if ('q' == cv::waitKey(20)) {
      // maybe you can call unsubscribe here
      mRadar_barrier_finder->UnSubscribeToRadar(1);
      break;
    }
  }
  allRadarsFile.close();
  // Reserve some time(here is 1 second) to let chassis heart beat thread,
  // Receive data thread exit.
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  return 0;
}
