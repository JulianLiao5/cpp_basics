/*************************************************************************
        > File Name: test_boost.cpp
        > Author:
        > Mail:
        > Created Time: 2019年04月08日 星期一 15时04分52秒
 ************************************************************************/

#include <iostream>
using namespace std;

#include <boost/signals2/signal.hpp>
#include <boost/signals2/shared_connection_block.hpp>

struct foo {
  void operator()() const;
};

void foo::operator()() const {
  std::cout << "foo" << std::endl;
}

struct bar {
  void operator()() const;
};

void bar::operator()() const {
  std::cout << ", bar" << std::endl;
}


void wowo() {
    std::cout << "wowo" << std::endl;
}

void haha() {
    std::cout << "haha" << std::endl;
}

int main(int argc, char *argv[]) {
  // Signal with no arguments and a void return value
  boost::signals2::signal<void()> sig;

  sig.connect(&wowo);
  sig.connect(&haha);
  sig();

  std::cout << "**11**" << std::endl;
  sig.disconnect(&haha);
  sig();

  return 0;
}
