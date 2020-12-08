#include <iostream>
#include <unistd.h>
// #include <memory>

#include <yaml-cpp/node/node.h>
#include <yaml-cpp/yaml.h>

#include <radar_barrier_range_finder.h>
#include <vehicle/can_obj.h>

#include <glog/logging.h>

int main(int argc, char *argv[]) {
  if (argc == 1 || (argc == 2 && strstr(argv[1], "-h"))) {
    std::cout << "Usage: " << argv[0] << " config_pod_radars.yaml" << std::endl;
    return -1;
  }

  YAML::Node user_config = YAML::LoadFile(argv[1]);
  PIAUTO::chassis::CanObj *CO = PIAUTO::chassis::CanObj::GetCanObj();

  // radar barrier parameters
  PIAUTO::perception::RadarBarrierRangeFinder::Parameters parameters(
      user_config, CO);

  // radar barrier finder
  PIAUTO::perception::RadarBarrierRangeFinder radar_barrier_finder(parameters);

  PIAUTO::perception::Perception_Obstacles pObstacles;

  while (1) {
    static int count = 0;
    count++;
    LOG(INFO) << "** " << count << " ** "
              << "BEGIN!";
    int result = radar_barrier_finder.GetObstacles(&pObstacles);
    if (0 == result) {
      LOG(INFO) << "[Success] obstacles num: " << pObstacles.obstacles.size()
                << std::endl;
      for (PIAUTO::perception::Perception_Obstacle obs_item :
           pObstacles.obstacles) {
        LOG(INFO) << obs_item << std::endl;
      }
    }
    LOG(INFO) << "** " << count << " ** "
              << "END!";
    usleep(20 * 1000);
  }

  return 0;
}
