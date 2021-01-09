#include <fusion_multi_sensors.h>
#include <testA.h>

#include <iostream>

class FusionMultiSensorObstacles::Impl {
public:
  Impl(const int &type_in);

  ~Impl();

  int GetObstacles(std::string &obs_str_out);

private:
  int m_type_;

  std::shared_ptr<TestA> test_a_;
};

FusionMultiSensorObstacles::FusionMultiSensorObstacles(const int &type_in)
    : impl_(std::make_shared<Impl>(type_in)) {}

FusionMultiSensorObstacles::~FusionMultiSensorObstacles() { impl_.reset(); }

int FusionMultiSensorObstacles::GetObstacles(std::string &obs_str) {
  return impl_->GetObstacles(obs_str);
}

FusionMultiSensorObstacles::Impl::Impl(const int &type_in) {
  m_type_ = type_in;
  test_a_ = std::make_shared<TestA>(m_type_);
}

FusionMultiSensorObstacles::Impl::~Impl() {
  std::cout << "[perception] FusionMultiSensorObstacles::Impl Destructed!"
            << std::endl;
}

int FusionMultiSensorObstacles::Impl::GetObstacles(std::string &obs_str_out) {
  int ret;
  std::string interface_str;
  bool loc_flag;
  if (nullptr != test_a_) {
    std::cout << __FUNCTION__ << __LINE__ << " call GetString" << std::endl;
    loc_flag = test_a_->GetString(interface_str);
  } else {
    std::cout << "test_a_ is NULL" << std::endl;
  }

  std::cout << "interface_str: " << interface_str << std::endl;

  switch (m_type_) {
  case 1: {
    obs_str_out = interface_str + "1111";
    break;
  }
  case 2: {
    obs_str_out = interface_str + "2222";
    break;
  }
  case 3: {
    obs_str_out = interface_str + "4444";
    break;
  }
  default: {
    obs_str_out = interface_str + "5555";
    break;
  }
  }

  return 1;
}