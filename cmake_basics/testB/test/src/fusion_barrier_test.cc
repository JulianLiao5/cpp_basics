
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <signal.h>

#include <fusion_multi_sensors.h>
#include <vehicle/can_obj.h>

static bool isExit = false;

DEFINE_string(chassis_config_path, "", "the file path of chassis_config.yaml");
DEFINE_string(sensor_config_path, "", "the file path of config_sensors.yaml");
DEFINE_string(chassis_type, "POD5", "vehicle_name");

static void intSigintHandler(int sig) {
  isExit = true;
  std::cout << "++++++++++++++ exit captured!" << std::endl;
}

int main(int argc, char *argv[]) {
  google::ParseCommandLineFlags(&argc, &argv, false);
  google::InstallFailureSignalHandler();
  // install SIGNAL handler
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = intSigintHandler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);

  PIAUTO::chassis::Chassis_Type chassis_id;
  if (!FLAGS_chassis_config_path.empty()) {
    PIAUTO::chassis::CanObj::InitFromYaml(YAML::LoadFile(FLAGS_chassis_config_path));
  } else if (!FLAGS_chassis_type.empty()) {
    chassis_id = PIAUTO::chassis::ConvertChassisNameToId(FLAGS_chassis_type);
    PIAUTO::chassis::CanObj::InitFromPresets(chassis_id);
    std::string chassis_name = PIAUTO::chassis::ConvertChassisIdToName(chassis_id);
    LOG(ERROR) << "No chassis config, provide chassis_type: " << chassis_name;
  } else {
    LOG(ERROR) << "Error, No chassis config, No chassis type!!!";
    return -1;
  }

  std::shared_ptr<YAML::Node> sensor_config;
  if (!FLAGS_sensor_config_path.empty()) {
    sensor_config = std::make_shared<YAML::Node>(YAML::LoadFile(FLAGS_sensor_config_path));
  }

  // int enable_vision =
  // sensor_config["perception_sensors"]["enable_vision"].as<int>();
  int enable_radar = 0;
  int enable_sonar = 0;
  if (nullptr != sensor_config) {
    enable_radar = (*sensor_config)["perception_sensors"]["enable_radar"].as<int>();
    enable_sonar = (*sensor_config)["perception_sensors"]["enable_sonar"].as<int>();
  }

  if (enable_radar == 0 && enable_sonar == 0) {
    // radar or sonar calls the CAN object, in which glog is inited
    std::cout << "neither radar or sonar is enabled, init glog" << std::endl;

    // level bigger than xx will be written to log
    FLAGS_minloglevel = 0;
    // level bigger than xx will be shown in terminal
    google::SetStderrLogging(4);
  }

  PIAUTO::chassis::CanObj *CO = PIAUTO::chassis::CanObj::GetCanObj();
  auto fusion_obstacles =
      std::make_shared<PIAUTO::perception::FusionMultiSensorObstacles>(FLAGS_sensor_config_path, chassis_id);
  auto radar_handler = fusion_obstacles->GetRadarHandler();
  for (int i = 0; i < 10; i++) {
    bool self_test_ret = CO->SelfTest();
    LOG(ERROR) << "i: " << i << "    self_test_ret: " << self_test_ret;
    std::cout << "i: " << i << "    self_test_ret: " << self_test_ret << std::endl;
  }

  piauto::perception::PerceptionObstacles p_obstacles;

  while (!isExit) {
    PIAUTO::perception::PerceptionStatus result = fusion_obstacles->GetObstacles(p_obstacles);

    std::cout << "fusion module: GetObstacles() vision_status: " << result.vision_status
              << " radar_status: " << result.radar_status << " sonar_status: " << result.sonar_status << std::endl;

    if (0 == result.vision_status && 0 == result.radar_status && 0 == result.sonar_status) {
      std::cout << "************* BEGIN  **************" << std::endl;
      for (auto const &object : p_obstacles.perception_obstacle()) {
        std::cout << "[sensor_id  obj_id  pose_x  pose_y  vel_x  vel_y  power]" << std::endl;
        std::cout << object.sensor_id() << " " << object.obstacle_id() << " " << object.position().x() << " "
                  << object.position().y() << " " << object.velocity().vel_x() << " " << object.velocity().vel_y()
                  << " " << object.power() << std::endl;
      }
      std::cout << "************* STOP  **************" << std::endl;
      std::cout << "\n\n\n";
    }

    // frequency: 10FPS
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  radar_handler = nullptr;
  fusion_obstacles = nullptr;

  return 0;
}
