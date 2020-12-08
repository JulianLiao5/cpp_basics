#include <signal.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
// #include <memory>

#include <boost/filesystem.hpp>

#include <yaml-cpp/node/node.h>
#include <yaml-cpp/yaml.h>

#include <occupancy_status_grid_map.h>
#include <radar_barrier_range_finder.h>
#include <vehicle/can_obj.h>

#include <gflags/gflags.h>
#include <glog/logging.h>

#ifdef INT
#undef INT
#endif

#ifdef UCHAR
#undef UCHAR
#endif

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

DEFINE_string(chassis_config_path, "", "the file path of chassis_config.yaml");
DEFINE_string(chassis_type, "S12", "vehicle_name");
// DEFINE_string(chassis_type, "POD5", "vehicle_name");
DEFINE_string(radar_config_path, "", "the file path of config_sensors.yaml");
DEFINE_string(ui_config_path, "", "the file path of config_ui.yaml");

std::ofstream allRadarsFile;

void DrawMap(double cell_size, int height, int width,
             std::shared_ptr<YAML::Node> ui_config,
             std::shared_ptr<OccupancyStatusGridMap> OGM) {
  OGM->Reset();

  int half_height_in_meters = static_cast<int>(height * cell_size / 2.0);
  int half_width_in_meters = static_cast<int>(width * cell_size / 2.0);

  std::vector<double> lanes_size;
  std::vector<double> marks_size;

  double specify_x = 0.0;
  double specify_y = 0.0;
  if (nullptr != ui_config) {
    lanes_size = PIAUTO::util::getVecFromNode<double>(
        (*ui_config)["visualization"]["lanes_size"]);
    marks_size = PIAUTO::util::getVecFromNode<double>(
        (*ui_config)["visualization"]["marks_size"]);
    specify_x = (*ui_config)["visualization"]["specify_line_base_on_x_value"]
                    .as<double>();
    specify_y = (*ui_config)["visualization"]["specify_line_base_on_y_value"]
                    .as<double>();
  } else {
    lanes_size.push_back(0.07);
    lanes_size.push_back(0.8);
    marks_size.push_back(0.2);
    marks_size.push_back(0.07);
    specify_x = 100.0;
    specify_y = -100.0;
  }

  for (int i = -2; i < half_width_in_meters; i++) {
    OGM->DrawLineInMap(cv::Point2d(i, -half_width_in_meters),
                       cv::Point2d(i, half_width_in_meters),
                       CellStatus::UNKNOWN);
  }

  OGM->DrawLineInMap(cv::Point2d(specify_x, -half_width_in_meters),
                     cv::Point2d(specify_x, half_width_in_meters),
                     CellStatus::UNKNOWN);
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

void AddTextToMap(double cell_size, int height, int width,
                  std::shared_ptr<YAML::Node> ui_config, cv::Mat m) {
  int half_height_in_meters = static_cast<int>(height * cell_size / 2.0);
  int half_width_in_meters = static_cast<int>(width * cell_size / 2.0);
  int half_height_in_pixels = static_cast<int>(height / 2.0);
  int half_width_in_pixels = static_cast<int>(width / 2.0);
  std::vector<int> viz_origin_offset;
  std::vector<int> viz_step;
  double viz_text_in_line;

  if (nullptr != ui_config) {
    viz_origin_offset = PIAUTO::util::getVecFromNode<int>(
        (*ui_config)["visualization"]["origin_offset"]);
    viz_step = PIAUTO::util::getVecFromNode<int>(
        (*ui_config)["visualization"]["marks_step"]);
    viz_text_in_line =
        (*ui_config)["visualization"]["text_location_ratio_in_line"]
            .as<double>();
  } else {
    viz_origin_offset.push_back(4);
    viz_origin_offset.push_back(-2);
    viz_step.push_back(100);
    viz_step.push_back(100);
    viz_text_in_line = 0.667;
  }

  // [BEGIN x-axis in map coordinate]
  for (int i = -(half_height_in_meters - 1); i < half_height_in_meters; i++) {
    cv::Point loc = cv::Point(
        half_height_in_pixels + viz_origin_offset[0],
        half_height_in_pixels - (i * viz_step[0]) + viz_origin_offset[1]);
    cv::putText(m, std::to_string(i), loc, cv::FONT_HERSHEY_DUPLEX, 1.0,
                CV_RGB(0, 0, 255), 2);
  }
  // [END x-axis in map coordinate]

  // [BEGIN y-axis in map coordinate]
  for (int i = -(half_width_in_meters - 2); i < (half_width_in_meters - 1);
       i++) {
    cv::Point loc = cv::Point(
        half_width_in_pixels - (i * viz_step[1]) + viz_origin_offset[0],
        half_height_in_pixels + viz_origin_offset[1]);
    cv::putText(m, std::to_string(i), loc, cv::FONT_HERSHEY_DUPLEX, 1.0,
                CV_RGB(0, 0, 255), 2);
  }
  // [END y-axis in map coordinate]

  // tan(30deg) = 0.577
  // tan(45deg) = 1
  // tan(60deg) = 1.732
  std::vector<int> degrees_in_max_x = {-45, -30, 30, 45};
  for (int deg_item_in_max_x : degrees_in_max_x) {
    cv::Point loc = cv::Point(
        static_cast<int>(half_width_in_pixels +
                         half_height_in_pixels * viz_text_in_line *
                             tan(deg_item_in_max_x * M_PI / 180.0)),
        static_cast<int>(half_height_in_pixels * (1 - viz_text_in_line)));
    cv::putText(m, std::to_string(deg_item_in_max_x) + "deg", loc,
                cv::FONT_HERSHEY_DUPLEX, 1.0, CV_RGB(0, 0, 255), 2);
  }
  std::vector<int> degrees_in_min_x = {-150, 150};
  for (int deg_item_in_min_x : degrees_in_min_x) {
    cv::Point loc = cv::Point(
        static_cast<int>(half_width_in_pixels -
                         half_height_in_pixels * viz_text_in_line *
                             tan(deg_item_in_min_x * M_PI / 180.0)),
        static_cast<int>(half_height_in_pixels * (1 + viz_text_in_line)));
    cv::putText(m, std::to_string(deg_item_in_min_x) + "deg", loc,
                cv::FONT_HERSHEY_DUPLEX, 1.0, CV_RGB(0, 0, 255), 2);
  }

  std::vector<int> degrees_in_min_y_max_y = {-135, -120, -60, 60, 120, 135};
  for (int deg_item_in_min_y_max_y : degrees_in_min_y_max_y) {
    cv::Point loc = cv::Point(
        static_cast<int>(
            half_width_in_pixels *
            (1 - ((deg_item_in_min_y_max_y < 0) ? 1 : -1) * viz_text_in_line)),
        static_cast<int>(
            half_height_in_pixels -
            half_width_in_pixels * viz_text_in_line /
                tan(std::abs(deg_item_in_min_y_max_y) * M_PI / 180)));
    cv::putText(m, std::to_string(deg_item_in_min_y_max_y) + "deg", loc,
                cv::FONT_HERSHEY_DUPLEX, 1.0, CV_RGB(0, 0, 255), 2);
  }
}

void exit_handler(int s) { printf("exit_hander!\n"); }

/**
 * radar's coordinate: x: front, y: left, z: up
 *         clockwise is +, [0deg, 69deg]
 *         counter-clockwise is -, [-69deg, 0deg)
 * map's coordinate: x: vertical, up+, down-; y: horizontal, left+, right-
 *     x: -10.0, 10.0
 *     y: -10.0, 10.0
 *         clockwise is +, [0deg, 180deg)
 *         counter-clockwise is -, [-180deg, 0deg)
 * image's coordinate:
 *         origin: left-top
 *     x: 0, 2000
 *     y: 0, 2000
 *
 * 0.01 * (Ximage - 1000) = Xmap
 *   x = 1000 Xmap = 0
 *   x = 2000 XMap = 10
 *         x points right, from left to right, [0, width], x <-> width
 *         y points down, from up to down, [0, height], y <-> height
 */
int main(int argc, char *argv[]) {
  google::InitGoogleLogging(argv[0]);
  std::string log_path = "./log_" + PIAUTO::time::getTimeStamps();
  if (!boost::filesystem::exists(log_path)) {
    boost::filesystem::create_directory(log_path);
  }
  if (boost::filesystem::is_directory(log_path)) {
    FLAGS_log_dir = log_path;
  }
  google::ParseCommandLineFlags(&argc, &argv, false);
  google::InstallFailureSignalHandler();
  // install SIGNAL handler
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = exit_handler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);

  std::shared_ptr<YAML::Node> radars_config = nullptr;
  std::shared_ptr<YAML::Node> ui_config = nullptr;

  std::shared_ptr<PIAUTO::perception::RadarBarrierRangeFinder>
      radar_barrier_finder;
  PIAUTO::chassis::CanObj *CO;

  if (!FLAGS_chassis_config_path.empty()) {
    PIAUTO::chassis::CanObj::InitFromYaml(
        YAML::LoadFile(FLAGS_chassis_config_path));
    CO = PIAUTO::chassis::CanObj::GetCanObj();
    if (!FLAGS_radar_config_path.empty()) {
      radars_config =
          std::make_shared<YAML::Node>(YAML::LoadFile(FLAGS_radar_config_path));
    }
    // radar barrier parameters
    PIAUTO::perception::RadarBarrierRangeFinder::Parameters parameters(
        (*radars_config), CO);

    // radar barrier finder
    radar_barrier_finder =
        std::make_shared<PIAUTO::perception::RadarBarrierRangeFinder>(
            parameters);
  } else if (!FLAGS_chassis_type.empty()) {
    PIAUTO::chassis::Chassis_Type chassis_id =
        PIAUTO::chassis::ConvertChassisNameToId(FLAGS_chassis_type);
    PIAUTO::chassis::CanObj::InitFromPresets(chassis_id);
    CO = PIAUTO::chassis::CanObj::GetCanObj();

    // radar barrier parameters
    PIAUTO::perception::RadarBarrierRangeFinder::Parameters parameters(
        CO, chassis_id);

    radar_barrier_finder =
        std::make_shared<PIAUTO::perception::RadarBarrierRangeFinder>(
            parameters);
  }

  if (!FLAGS_ui_config_path.empty()) {
    ui_config =
        std::make_shared<YAML::Node>(YAML::LoadFile(FLAGS_ui_config_path));
  }

  PIAUTO::perception::Perception_Obstacles pObstacles;

  double size_per_pixel = 0.01;
  int height = 3200;
  int width = 3200;
  double obj_circle_radius = 20.0;
  int text_size = 24;

  if (nullptr != ui_config) {
    size_per_pixel =
        (*ui_config)["occupancy_grid_map"]["cell_size"].as<double>();
    std::vector<int> grid_size = PIAUTO::util::getVecFromNode<int>(
        (*ui_config)["occupancy_grid_map"]["grid_size"]);

    height = grid_size[0];
    width = grid_size[1];

    obj_circle_radius =
        (*ui_config)["visualization"]["obj_circle_radius"].as<double>();
    text_size = (*ui_config)["visualization"]["text_size"].as<int>();
  }

  cv::Size map_size(height, width);
  std::shared_ptr<OccupancyStatusGridMap> OGM =
      std::make_shared<OccupancyStatusGridMap>(size_per_pixel, map_size);
  allRadarsFile.open("./all_radars_data.txt", std::ios::out);

  while (1) {
#if DEBUG
    std::thread::id main_id = std::this_thread::get_id();
    cout << "[" << __func__ << "] in thread(" << main_id << ")" << endl;
#endif

    DrawMap(size_per_pixel, height, width, ui_config, OGM);

    cv::Mat m = OGM->Visualize();
    cv::namedWindow("map", cv::WINDOW_NORMAL);
    static int count = 0;
    count++;
    LOG(INFO) << "** " << count << " ** "
              << "BEGIN!";

    int result = radar_barrier_finder->GetObstacles(&pObstacles);

    if (0 == result) {
      LOG(INFO) << "[Success] obstacles num: " << pObstacles.obstacles.size()
                << std::endl;
      cv::putText(m, std::to_string(CO->GetVCU().GetSpeed()),
                  cv::Point2d(300, 300), cv::FONT_HERSHEY_DUPLEX, 1.0,
                  CV_RGB(255, 0, 0), 2);
      for (PIAUTO::perception::Perception_Obstacle obs_item :
           pObstacles.obstacles) {
        LOG(INFO) << obs_item << std::endl;
        allRadarsFile << obs_item;
        // transform the position from meter unit to pixel unit
        double x_image = width * 0.5 - obs_item.pose.y / size_per_pixel;
        double y_image = height * 0.5 - obs_item.pose.x / size_per_pixel;
        cv::circle(m, cv::Point2d(x_image, y_image), obj_circle_radius,
                   cv::Scalar(0, 0, 255), cv::FILLED, cv::LINE_AA);
        cv::putText(m, std::to_string(obs_item.sensor_id) + "," +
                           std::to_string(obs_item.obj_id),
                    cv::Point2d(x_image - 5, y_image + 5),
                    cv::FONT_HERSHEY_DUPLEX, 1.0, CV_RGB(255, 0, 0), 2);
      }
    }

    LOG(INFO) << "** " << count << " ** "
              << "END!";

    // cv::namedWindow("map", cv::WINDOW_AUTOSIZE);
    AddTextToMap(size_per_pixel, height, width, ui_config, m);
    // cv::namedWindow("map", cv::WINDOW_NORMAL);
    /* if (car.data != nullptr) {
      cv::Rect roi_rect(OGM->grid_size().width / 2 - car.cols / 2,
    OGM->grid_size().height / 2 - car.rows / 2, car.cols, car.rows);
      car.copyTo(m(roi_rect));
    } else {
      OGM->DrawRectInMap(Rect(0, 0, 1, 1.6));
    } */
    cv::imshow("map", m);
    if ('q' == cv::waitKey(20)) {
      LOG(ERROR) << "q is pressed!";
      break;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(25));
  }  // end of 'while (1)'

  radar_barrier_finder = nullptr;
  allRadarsFile.close();
  // Reserve some time(here is 1 second) to let chassis heart beat thread,
  // Receive data thread exit.
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  return 0;
}
