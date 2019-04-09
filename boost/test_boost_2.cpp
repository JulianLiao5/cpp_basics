/*************************************************************************
        > File Name: test_boost.cpp
        > Author:
        > Mail:
        > Created Time: 2019年04月08日 星期一 15时04分52秒
 ************************************************************************/

#include <iostream>
using namespace std;

#include <boost/signals2/signal.hpp>

struct Hello {
  void operator()() const;
};

void Hello::operator()() const {
  std::cout << "Kyra" << std::endl;
}

struct World {
  void operator()() const;
};

void World::operator()() const {
  std::cout << ", Julian!" << std::endl;
}

int main(int argc, char *argv[]) {
  // Signal with no arguments and a void return value
  boost::signals2::signal<void ()> sig;

  // Connect a HelloWorld slot
  sig.connect(Hello());
  sig.connect(World());

  // Call all of the slots
  sig();

  return 0;
}
