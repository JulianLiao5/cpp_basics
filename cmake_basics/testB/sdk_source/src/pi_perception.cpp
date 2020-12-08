
#include <pi_perception.h>

PIMultiSensorsPerception::PIMultiSensorsPerception(const int &type_) {
  sensor_obstacles_ = std::make_shared<FusionMultiSensorObstacles>(type_);
}

int PIMultiSensorsPerception::GetObstacles(std::string &output_obs_str) {
  return sensor_obstacles_->GetObstacles(output_obs_str);
}