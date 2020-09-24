/*************************************************************************
	> File Name: test_try_lock.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年02月01日 星期四 19时08分38秒
 ************************************************************************/

#include<iostream>
#include <thread>    // std::thread
#include <mutex>    // std::mutex, std::try_lock
#include <vector>    // std::vector


using namespace std;


// ------------[Begin std::try_lock] ===============
// prototype:
    // template <class Mutex1, class Mutex2, class... Mutexes> int try_lock(Mutex1& a, Mutex2& b, Mutexes&... cde);
    // 尝试锁多个mutex对象
    // 这个函数按照参数顺序去调用每一个参数的try_lock函数，会有2种结果，
    // 其一，所有参数的try_lock都锁成功了，返回-1
    // 其二，有一个参数的try_lock没有锁成功，那么已经锁成功的mutex对象会通过调用unlock先将锁释放掉，
    //         同时会返回这个锁失败的mutex对象在参数中的index已表明是哪个mutex对象锁失败了。
// ------------[End std::try_lock] ===============


// ------------[Begin std::unique_lock::owns_lock] ===============
// prototype:
    // bool owns_lock() const noexcept;
    // 如果unique_lock所管理的mutex对象被锁住了并且还没有解锁的话，那么就返回true
// ------------[End std::unique_lock::owns_lock] ===============





std::mutex foo, bar;
std::mutex mtx;

void print_star() {
    std::unique_lock<std::mutex> lck(mtx, std::try_to_lock);
    // print '*' if successfully locked, 'x' otherwise
    if (lck.owns_lock()) {
        cout << '*';
    } else {
        cout << 'x';
    }
}

void task_a() {
  foo.lock();
  std::cout << "task a\n";
  bar.lock();

  foo.unlock();
  bar.unlock();
}

void task_b() {
  int x = try_lock(bar, foo);
  // x有3种可能返回值：-1, 0, 1
  cout << "x: " << x << endl;

    if (x == -1) {
      cout << "task b try_lock success\n";

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


    std::vector<std::thread> threads;
    for (int i = 0; i < 500; i++) {
       threads.emplace_back(print_star);
    }

    for (std::thread& th : threads) {
      th.join();
    }

    return 0;
}
