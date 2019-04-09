/*************************************************************************
        > File Name: test_boost.cpp
        > Author:
        > Mail:
        > Created Time: 2019年04月08日 星期一 15时04分52秒
 ************************************************************************/

#include <iostream>
using namespace std;

#include <boost/signals2/signal.hpp>

void print_args(float x, float y) {
  std::cout << "The arguments are " << x << " and " << y << std::endl;
}

void print_sum(float x, float y) {
  std::cout << "The sum is " << x + y << std::endl;
}

void print_product(float x, float y) {
  std::cout << "The product are " << x * y << std::endl;
}

void print_quotient(float x, float y) {
  std::cout << "The quotient is " << x / y << std::endl;
}

int main(int argc, char *argv[]) {
  // Signal with no arguments and a void return value
  boost::signals2::signal<void (float, float)> sig;

  // Connect a HelloWorld slot
  sig.connect(&print_args);
  sig.connect(&print_sum);
  sig.connect(&print_product);
  sig.connect(&print_quotient);

  // Call all of the slots
  sig(5., 3.);

  return 0;
}
