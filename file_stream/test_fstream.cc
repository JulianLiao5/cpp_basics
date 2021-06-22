#include <fstream>
#include <sstream>

#include <glog/logging.h>

int main(int argc, char *argv[]) {
    FLAGS_log_dir = "./";
    google::InitGoogleLogging(argv[0]);
    std::ifstream calib_file("./calib_table.txt", std::ifstream::binary);
    if (!calib_file) {
        LOG(ERROR) << "Open calib_table.txt fail, please check the file exist or not!";
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
    // // calib_str.assign((std::istreambuf_iterator<char>()), std::istreambuf_iterator<char>());

    size_t len = calib_str.length();
    LOG(INFO) << "len: " << len;
    LOG(INFO) << calib_str << "1111111111111111111";

    return 0;
}
