
#include <memory>

class FusionMultiSensorObstacles {
public:
  FusionMultiSensorObstacles(const int &type);
  virtual ~FusionMultiSensorObstacles();

  // return protobuf shared_ptr
  int GetObstacles(std::string &obs_str);

private:
  class Impl;
  std::shared_ptr<Impl> impl_;
};
