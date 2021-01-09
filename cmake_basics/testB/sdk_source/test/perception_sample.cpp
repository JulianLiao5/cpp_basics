#include <csignal>
#include <fstream>
#include <iomanip>
#include <iostream>

#include <pi_perception.h>

static bool kIsExit = false;

static void SignalHandler(int sig) {
  kIsExit = true;
  std::cout << "system closed" << std::endl;
}

int main(int argc, char *argv[]) {
  std::signal(SIGINT, SignalHandler);
  std::signal(SIGTERM, SignalHandler);
  std::signal(SIGUSR1, SignalHandler);

  int test_num = 3;
  auto fusion_obstacles = std::make_shared<PIMultiSensorsPerception>(test_num);

  std::string result_str;

  int ret_int = fusion_obstacles->GetObstacles(result_str);

  std::cout << "result_str: " << result_str << std::endl;

  fusion_obstacles = nullptr;

  return 0;
}