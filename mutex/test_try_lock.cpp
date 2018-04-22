/*************************************************************************
	> File Name: test_try_lock.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年02月01日 星期四 19时08分38秒
 ************************************************************************/

#include<iostream>
#include <thread>    // std::thread
#include <mutex>    // std::mutex, std::try_lock


using namespace std;

std::mutex foo, bar;


void task_a() {
  foo.lock();
  std::cout << "task a\n";
  bar.lock();

  foo.unlock();
  bar.unlock();
}

void task_b() {
  int x = try_lock(bar, foo);
  cout << "x: " << x << endl;

    if (x == -1) {
      cout << "task b\n";

      bar.unlock();
      foo.unlock();
    } else {
      // index of "bar": 0
      // index of "foo": 1
      cout << "[task b failed: mutex " << (x ? "foo" : "bar") << " locked]\n";
    }
}


int main(int argc, char** argv) {
    std::thread th1(task_a);
    std::thread th2(task_b);

    th1.join();
    th2.join();

    return 0;
}
