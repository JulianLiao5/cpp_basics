#pragma once

#include<memory>
#include <fusion_multi_sensors.h>

// forward declaration the internal class
class FusionMultiSensorObstacles;

class PIMultiSensorsPerception {
 public:
  PIMultiSensorsPerception(const int &type_);

  int GetObstacles(std::string &);

 private:
  std::shared_ptr<FusionMultiSensorObstacles> sensor_obstacles_ = nullptr;
};
