
#include <iostream>
#include <memory>
#include <string>

#include "testA.h"

int main(int argc, char **argv) {

  std::cout << "***********__GNUC__: " << __GNUC__ << "\n";

  int local_type = 2;
  std::shared_ptr<TestA> testA = std::make_shared<TestA>(local_type);

  std::string test_data;
  bool result = testA->GetString(test_data);
  if (!result) {
    std::cout << "GetString error!\n";
  } else {
    std::cout << "GetString ok, test_data: " << test_data << std::endl;
  }
  return 0;
}
