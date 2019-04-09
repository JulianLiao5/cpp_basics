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
  std::cout << "c is not blocked\n";
  sig();

    {
        boost::signals2::shared_connection_block block(c);
        std::cout << "c is blocked!\n";
        sig();  // No output: the slot is blocked
    }  // shared_connection_block going out of scope unblocks the slot

  std::cout << "c is not blocked\n";
  sig();

  return 0;
}
