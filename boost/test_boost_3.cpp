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

struct Love {
  void operator()() const;
};

void Love::operator()() const {
  std::cout << "Love" << std::endl;
}
struct World {
  void operator()() const;
};

void World::operator()() const {
  std::cout << ", Julian!" << std::endl;
}

struct Good {
  void operator()() const;
};

void Good::operator()() const {
  std::cout << "**Good**" << std::endl;
}

struct Bad {
  void operator()() const;
};

void Bad::operator()() const {
  std::cout << "**Bad**" << std::endl;
}

int main(int argc, char *argv[]) {
  // Signal with no arguments and a void return value
  boost::signals2::signal<void ()> sig;

  // Connect a HelloWorld slot
  sig.connect(1, Hello());
  sig.connect(1, Love());
  sig.connect(0, World());
  sig.connect(Good());
  sig.connect(Bad(), boost::signals2::at_front);

  // Call all of the slots
  sig();

  return 0;
}
