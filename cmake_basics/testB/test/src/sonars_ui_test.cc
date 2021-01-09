#include <signal.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
// #include <memory>

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <occupancy_status_grid_map.h>
#include <sonar_barrier_range_finder.h>
#include <vehicle/can_obj.h>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/yaml.h>

#include <boost/filesystem.hpp>

#ifdef INT
#undef INT
#endif

#ifdef UCHAR
#undef UCHAR
#endif

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

DEFINE_string(chassis_config_path, "", "the file path of chassis_config.yaml");
DEFINE_string(chassis_type, "POD5", "vehicle_name");
DEFINE_string(sonar_config_path, "", "the file path of config_sensors.yaml");
DEFINE_string(ui_config_path, "", "the file path of config_ui.yaml");

std::ofstream allSonarsFile;

void ReDrawMap(double cell_size, int height, int width,
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
    // void cv::putText(InputOutputArray img, const String& text, Point org, int
    // fontFace, double fontScale, Scalar
    // color, int thickness = 1, int lineType = LINE_8, bool bottomLeftOrigin =
    // false); cv::putText(img, "Hello,
    // OpenCV!", cv::Point(10, img.rows / 2), cv::FONT_HERSHEY_DUPLEX, 1.0,
    // CV_RGB(118, 185, 0), 2);
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
}

bool is_exit = false;

void exit_handler(int s) {
  is_exit = true;
  printf("exit_hander!\n");
}

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

  std::shared_ptr<YAML::Node> sonars_config = nullptr;
  std::shared_ptr<YAML::Node> ui_config = nullptr;

  std::shared_ptr<PIAUTO::perception::SonarBarrierRangeFinder>
      sonar_barrier_finder;
  PIAUTO::chassis::Chassis_Type chassis_id;
  PIAUTO::chassis::CanObj *CO;

  if (!FLAGS_chassis_config_path.empty()) {
    PIAUTO::chassis::CanObj::InitFromYaml(
        YAML::LoadFile(FLAGS_chassis_config_path));
    // sonar barrier parameters
    PIAUTO::perception::SonarBarrierRangeFinder::Parameters parameters(
        (*sonars_config), CO);

    // sonar barrier finder
    sonar_barrier_finder =
        std::make_shared<PIAUTO::perception::SonarBarrierRangeFinder>(
            parameters);
  } else if (!FLAGS_chassis_type.empty()) {
    chassis_id = PIAUTO::chassis::ConvertChassisNameToId(FLAGS_chassis_type);
    PIAUTO::chassis::CanObj::InitFromPresets(chassis_id);
    CO = PIAUTO::chassis::CanObj::GetCanObj();

    // sonar barrier parameters
    PIAUTO::perception::SonarBarrierRangeFinder::Parameters parameters(
        CO, chassis_id);

    sonar_barrier_finder =
        std::make_shared<PIAUTO::perception::SonarBarrierRangeFinder>(
            parameters);
  }

  if (!FLAGS_sonar_config_path.empty()) {
    sonars_config =
        std::make_shared<YAML::Node>(YAML::LoadFile(FLAGS_sonar_config_path));
  }
  if (!FLAGS_ui_config_path.empty()) {
    ui_config =
        std::make_shared<YAML::Node>(YAML::LoadFile(FLAGS_ui_config_path));
  }

  std::vector<int> mSonarIds;
  std::vector<PIAUTO::perception::SensorOffset> sensors_offset;
  std::vector<Eigen::Matrix3d> T_vehicle_sonars;
  float horizontalAngle = 10.0;
  std::vector<int> grid_size;
  double size_per_pixel = 0.01;
  int height = 1200;
  int width = 1200;

  if (nullptr != sonars_config && nullptr != ui_config) {
    size_per_pixel =
        (*ui_config)["occupancy_grid_map"]["cell_size"].as<double>();

    grid_size = PIAUTO::util::getVecFromNode<int>(
        (*ui_config)["occupancy_grid_map"]["grid_size"]);
    height = grid_size[0];
    width = grid_size[1];

    mSonarIds = PIAUTO::util::getVecFromNode<int>((*sonars_config)["sonarIds"]);

    for (int i = 0; i < mSonarIds.size(); i++) {
      std::string key_vehicle_sonar =
          "T_vehicle_sonar" + std::to_string(mSonarIds[i]);
      T_vehicle_sonars.push_back(PIAUTO::util::getEigenMatrix3dFromNode(
          (*sonars_config)[key_vehicle_sonar]));
    }

    horizontalAngle = (*sonars_config)["sonar"]["horizontal_angle"].as<float>();
  } else {  // use hardcode config
    int detector_num = 8;
    switch (chassis_id) {
      case PIAUTO::chassis::Chassis_Type::S8:  // done
        detector_num = 8;
        sensors_offset.push_back({2.96, 0.385, 0.0});
        sensors_offset.push_back({2.96, -0.385, 0.0});
        sensors_offset.push_back({2.10, -0.74, 90.0});
        sensors_offset.push_back({-0.44, -0.735, 90.0});
        sensors_offset.push_back({-0.79, -0.283, 180.0});
        sensors_offset.push_back({-0.79, 0.283, 180.0});
        sensors_offset.push_back({-0.44, 0.735, 270.0});
        sensors_offset.push_back({2.10, 0.76, 270.0});
        break;
      case PIAUTO::chassis::Chassis_Type::POD4:  // done
        detector_num = 8;
        sensors_offset.push_back({1.847, 0.253, 0.0});
        sensors_offset.push_back({1.847, -0.252, 0.0});
        sensors_offset.push_back({1.639, -0.722, 90.0});
        sensors_offset.push_back({-0.193, -0.725, 90.0});
        sensors_offset.push_back({-0.337, -0.255, 180.0});
        sensors_offset.push_back({-0.337, 0.252, 180.0});
        sensors_offset.push_back({-0.192, 0.717, 270.0});
        sensors_offset.push_back({1.639, 0.726, 270.0});
        break;

      case PIAUTO::chassis::Chassis_Type::POD5:  // done
        detector_num = 8;
        sensors_offset.push_back({1.812, 0.352, 0.0});
        sensors_offset.push_back({1.812, -0.352, 0.0});
        sensors_offset.push_back({1.478, -0.595, 90.0});
        sensors_offset.push_back({-0.157, -0.575, 90.0});
        sensors_offset.push_back({-0.375, -0.313, 180.0});
        sensors_offset.push_back({-0.375, 0.313, 180.0});
        sensors_offset.push_back({-0.157, 0.575, 270.0});
        sensors_offset.push_back({1.478, 0.595, 270.0});
        break;

      case PIAUTO::chassis::Chassis_Type::POD6:  //
        detector_num = 8;
        sensors_offset.push_back({1.812, 0.352, 0.0});
        sensors_offset.push_back({1.812, -0.352, 0.0});
        sensors_offset.push_back({1.478, -0.595, 90.0});
        sensors_offset.push_back({-0.157, -0.575, 90.0});
        sensors_offset.push_back({-0.375, -0.313, 180.0});
        sensors_offset.push_back({-0.375, 0.313, 180.0});
        sensors_offset.push_back({-0.157, 0.575, 270.0});
        sensors_offset.push_back({1.478, 0.595, 270.0});
        break;

      case PIAUTO::chassis::Chassis_Type::VENDING2:  //
        detector_num = 8;
        sensors_offset.push_back({1.322, 0.251, 0.0});
        sensors_offset.push_back({1.322, -0.25, 0.0});
        sensors_offset.push_back({0.853, -0.474, 90.0});
        sensors_offset.push_back({0.154, -0.474, 90.0});
        sensors_offset.push_back({-0.276, -0.253, 180.0});
        sensors_offset.push_back({-0.276, 0.249, 180.0});
        sensors_offset.push_back({0.159, 0.475, 270.0});
        sensors_offset.push_back({0.858, 0.475, 270.0});
        break;

      case PIAUTO::chassis::Chassis_Type::S12:
        detector_num = 8;
        sensors_offset.push_back({3.12, 0.428, 0.0});
        sensors_offset.push_back({3.12, -0.428, 0.0});
        sensors_offset.push_back({1.805, -0.767, 90.0});
        sensors_offset.push_back({-0.20, -0.752, 90.0});
        sensors_offset.push_back({-1.06, -0.441, 180.0});
        sensors_offset.push_back({-1.06, 0.441, 180.0});
        sensors_offset.push_back({-0.20, 0.752, 270.0});
        sensors_offset.push_back({1.805, 0.767, 270.0});
        break;

      case PIAUTO::chassis::Chassis_Type::HUNTER:
        detector_num = 4;
        sensors_offset.push_back({0.801, 0.0, 0.0});
        sensors_offset.push_back({0.275, -0.335, 90.0});
        sensors_offset.push_back({-0.264, 0.102, 180.0});
        sensors_offset.push_back({0.275, 0.335, 270.0});
        break;

      default:  //
        break;
    }

    for (int j = 0; j < detector_num; j++) {
      mSonarIds.push_back(j);
    }

    for (PIAUTO::perception::SensorOffset offset_item : sensors_offset) {
      double th_in_rad = offset_item.offset_theta * M_PI / 180;
      Eigen::Matrix3d T_vehicle_sonar_tmp;
      T_vehicle_sonar_tmp << cosf(th_in_rad), sinf(th_in_rad),
          offset_item.offset_x, -sinf(th_in_rad), cosf(th_in_rad),
          offset_item.offset_y, 0.0, 0.0, 1.0;
      T_vehicle_sonars.push_back(T_vehicle_sonar_tmp);
    }
  }
  std::stringstream ss;
  ss << "sonarIds[" << mSonarIds.size() << "] = {";
  for (int id : mSonarIds) {
    ss << id << ", ";
  }
  ss << "}" << std::endl;
  LOG(INFO) << ss.str();

  std::vector<Eigen::Vector3d> coordinate_in_sonar(mSonarIds.size());
  std::vector<Eigen::Vector3d> coordinate_in_vehicle(mSonarIds.size());

  PIAUTO::perception::Perception_Obstacles pObstacles;

  cv::Size map_size(height, width);
  std::shared_ptr<OccupancyStatusGridMap> OGM =
      std::make_shared<OccupancyStatusGridMap>(size_per_pixel, map_size);
  allSonarsFile.open("./all_sonars_data.txt", std::ios::out);

  while (!is_exit) {
#if DEBUG
    std::thread::id main_id = std::this_thread::get_id();
    cout << "[" << __func__ << "] in thread(" << main_id << ")" << endl;
#endif

    ReDrawMap(size_per_pixel, height, width, ui_config, OGM);
    static int count = 0;
    count++;
    LOG(INFO) << "** " << count << " ** "
              << "BEGIN!";

    int result = sonar_barrier_finder->GetObstacles(&pObstacles);
    if (0 == result) {
      LOG(INFO) << "[Success] obstacles num: " << pObstacles.obstacles.size()
                << std::endl;

      for (PIAUTO::perception::Perception_Obstacle obs_item :
           pObstacles.obstacles) {
        LOG(INFO) << obs_item << std::endl;
        allSonarsFile << obs_item;

        std::vector<int>::iterator it =
            std::find(mSonarIds.begin(), mSonarIds.end(), obs_item.sensor_id);
        // Get index of sonar_Id from mSonarIds
        int index = std::distance(mSonarIds.begin(), it);

        double start_x = 0.0;
        double start_y = 0.0;
        double end_x = 0.0;
        double end_y = 0.0;

        switch (obs_item.sensor_id) {
          case 0: {
            if (PIAUTO::chassis::Chassis_Type::HUNTER == chassis_id) {
              start_x = T_vehicle_sonars[index](0, 2) + obs_item.arc.radius;
              start_y = T_vehicle_sonars[index](1, 2);
            } else {
              start_x = T_vehicle_sonars[index](0, 2) +
                        obs_item.arc.radius *
                            cosf((horizontalAngle / 2.0 / 180.0) * M_PI);
              start_y = T_vehicle_sonars[index](1, 2) +
                        obs_item.arc.radius *
                            sinf((horizontalAngle / 2.0 / 180.0) * M_PI);
            }
            break;
          }

          case 1: {
            if (PIAUTO::chassis::Chassis_Type::HUNTER == chassis_id) {
              start_x = T_vehicle_sonars[index](0, 2);
              start_y = T_vehicle_sonars[index](1, 2) - obs_item.arc.radius;
            } else {
              start_x = T_vehicle_sonars[index](0, 2) +
                        obs_item.arc.radius *
                            cosf((horizontalAngle / 2.0 / 180.0) * M_PI);
              start_y = T_vehicle_sonars[index](1, 2) +
                        obs_item.arc.radius *
                            sinf((horizontalAngle / 2.0 / 180.0) * M_PI);
            }
            break;
          }

          case 2: {
            if (PIAUTO::chassis::Chassis_Type::HUNTER == chassis_id) {
              start_x = T_vehicle_sonars[index](0, 2) - obs_item.arc.radius;
              start_y = T_vehicle_sonars[index](1, 2);
            } else {
              start_x = T_vehicle_sonars[index](0, 2) +
                        obs_item.arc.radius *
                            sinf((horizontalAngle / 2.0 / 180.0) * M_PI);
              start_y = T_vehicle_sonars[index](1, 2) -
                        obs_item.arc.radius *
                            cosf((horizontalAngle / 2.0 / 180.0) * M_PI);
            }
            break;
          }

          case 3: {
            if (PIAUTO::chassis::Chassis_Type::HUNTER == chassis_id) {
              start_x = T_vehicle_sonars[index](0, 2);
              start_y = T_vehicle_sonars[index](1, 2) + obs_item.arc.radius;
            } else {
              start_x = T_vehicle_sonars[index](0, 2) +
                        obs_item.arc.radius *
                            sinf((horizontalAngle / 2.0 / 180.0) * M_PI);
              start_y = T_vehicle_sonars[index](1, 2) -
                        obs_item.arc.radius *
                            cosf((horizontalAngle / 2.0 / 180.0) * M_PI);
            }
            break;
          }

          case 4: {
            start_x = T_vehicle_sonars[index](0, 2) -
                      obs_item.arc.radius *
                          cosf((horizontalAngle / 2.0 / 180.0) * M_PI);
            start_y = T_vehicle_sonars[index](1, 2) -
                      obs_item.arc.radius *
                          sinf((horizontalAngle / 2.0 / 180.0) * M_PI);
            end_x = T_vehicle_sonars[index](0, 2) -
                    obs_item.arc.radius *
                        cosf((horizontalAngle / 2.0 / 180.0) * M_PI);
            end_y = T_vehicle_sonars[index](1, 2) +
                    obs_item.arc.radius *
                        sinf((horizontalAngle / 2.0 / 180.0) * M_PI);
            break;
          }

          case 5: {
            start_x = T_vehicle_sonars[index](0, 2) -
                      obs_item.arc.radius *
                          cosf((horizontalAngle / 2.0 / 180.0) * M_PI);
            start_y = T_vehicle_sonars[index](1, 2) -
                      obs_item.arc.radius *
                          sinf((horizontalAngle / 2.0 / 180.0) * M_PI);
            end_x = T_vehicle_sonars[index](0, 2) -
                    obs_item.arc.radius *
                        cosf((horizontalAngle / 2.0 / 180.0) * M_PI);
            end_y = T_vehicle_sonars[index](1, 2) +
                    obs_item.arc.radius *
                        sinf((horizontalAngle / 2.0 / 180.0) * M_PI);
            break;
          }

          case 6: {
            start_x = T_vehicle_sonars[index](0, 2) -
                      obs_item.arc.radius *
                          sinf((horizontalAngle / 2.0 / 180.0) * M_PI);
            start_y = T_vehicle_sonars[index](1, 2) +
                      obs_item.arc.radius *
                          cosf((horizontalAngle / 2.0 / 180.0) * M_PI);
            end_x = T_vehicle_sonars[index](0, 2) +
                    obs_item.arc.radius *
                        sinf((horizontalAngle / 2.0 / 180.0) * M_PI);
            end_y = T_vehicle_sonars[index](1, 2) +
                    obs_item.arc.radius *
                        cosf((horizontalAngle / 2.0 / 180.0) * M_PI);
            break;
          }

          case 7: {
            start_x = T_vehicle_sonars[index](0, 2) -
                      obs_item.arc.radius *
                          sinf((horizontalAngle / 2.0 / 180.0) * M_PI);
            start_y = T_vehicle_sonars[index](1, 2) +
                      obs_item.arc.radius *
                          cosf((horizontalAngle / 2.0 / 180.0) * M_PI);
            end_x = T_vehicle_sonars[index](0, 2) +
                    obs_item.arc.radius *
                        sinf((horizontalAngle / 2.0 / 180.0) * M_PI);
            end_y = T_vehicle_sonars[index](1, 2) +
                    obs_item.arc.radius *
                        cosf((horizontalAngle / 2.0 / 180.0) * M_PI);
            break;
          }

          default: { break; }
        }

        LOG(INFO) << "obs_item: sensor_id: " << obs_item.sensor_id
                  << ", index: " << index << ", start_x: " << start_x
                  << ", start_y: " << start_y;
        OGM->DrawRectInMap(Rect(start_x, start_y, 0.30, 0.28));
        // OGM->DrawLineInMap(cv::Point2d(start_x, start_y),
        // cv::Point2d(end_x, end_y));
      }
    }
    LOG(INFO) << "** " << count << " ** "
              << "END!";

    cv::Mat m = OGM->Visualize();
    cv::namedWindow("map", cv::WINDOW_NORMAL);
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

    switch (chassis_id) {
      // 1ECU_4sensors
      case PIAUTO::chassis::Chassis_Type::S8:
      case PIAUTO::chassis::Chassis_Type::POD5:
      case PIAUTO::chassis::Chassis_Type::POD6:
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
        break;
      // 1ECU_2sensors
      case PIAUTO::chassis::Chassis_Type::POD4:
      case PIAUTO::chassis::Chassis_Type::VENDING2:
        std::this_thread::sleep_for(std::chrono::milliseconds(70));
        break;

      default:  //
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
        break;
    }
  }  // end of 'while (1)'

  LOG(INFO) << "exiting...";
  sonar_barrier_finder = nullptr;
  allSonarsFile.close();
  // Reserve some time(here is 1 second) to let chassis heart beat thread,
  // Receive data thread exit.
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  return 0;
}
