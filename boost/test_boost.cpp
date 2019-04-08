/*************************************************************************
        > File Name: test_boost.cpp
        > Author:
        > Mail:
        > Created Time: 2019年04月08日 星期一 15时04分52秒
 ************************************************************************/

#include <iostream>
using namespace std;

#include <boost/signals2/signal.hpp>

struct HelloWorld {
  void operator()() const;
};

void HelloWorld::operator()() const {
  std::cout << "Hello, liaomeng!" << std::endl;
}

int main(int argc, char *argv[]) {
  // Signal with no arguments and a void return value
  boost::signals2::signal<void()> sig;

  // Connect a HelloWorld slot
  HelloWorld hello;
  sig.connect(hello);

  // Call all of the slots
  sig();

  return 0;
}
