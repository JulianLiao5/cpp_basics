
#include <google/protobuf/text_format.h>

#include <pwd.h>
#include <sys/types.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <glog/logging.h>

#include "config_calibration_table.pb.h"

using namespace std;

int main(int argc, char *argv[]) {
  piauto::chassis::AccelerationPid m_proto_calib_table_;

  FLAGS_log_dir = "./";
  google::InitGoogleLogging(argv[0]);

  struct passwd *pw = getpwuid(getuid());
  const char *homedir = pw->pw_dir;
  std::string calib_file_path = std::string(homedir) + "/calib_table.txt";
  LOG(INFO) << "calib_file_path: " << calib_file_path;
  std::ifstream calib_file(calib_file_path, std::ifstream::binary);
  if (!calib_file) {
    LOG(ERROR) << "Open " << calib_file_path
               << " fail, please check the file exist or not!";
    return -1;
  }
  if (calib_file.peek() == std::ifstream::traits_type::eof()) {
    LOG(ERROR) << "Open calib_table.txt success, but the file is empty!";
    return -1;
  }

  LOG(INFO) << "calib_file size1: " << calib_file.gcount();
  calib_file.seekg(0, calib_file.end);
  LOG(INFO) << "calib_file size2: " << calib_file.gcount();
  size_t file_size = calib_file.tellg();
  LOG(INFO) << "calib_file size3: " << file_size;
  std::string calib_str(file_size, ' ');
  calib_file.seekg(0);
  calib_file.read(&calib_str[0], file_size);
  // // calib_str.assign((std::istreambuf_iterator<char>()),
  // std::istreambuf_iterator<char>());

  size_t len = calib_str.length();
  LOG(INFO) << "len: " << len;
  LOG(INFO) << calib_str << "1111111111111111111";

  google::protobuf::TextFormat::ParseFromString(calib_str,
                                                &m_proto_calib_table_);
  LOG(INFO) << "AccelerationPid.calibration_table.calibration_info_size: "
            << m_proto_calib_table_.calibration_table().calibration_info_size();

  return 0;
}
