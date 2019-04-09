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

  boost::signals2::connection c = sig.connect(HelloWorld());
  std::cout << "c is connected\n";
  sig();

  c.disconnect();
  std::cout << "c is disconnected\n";
  sig();

  return 0;
}
