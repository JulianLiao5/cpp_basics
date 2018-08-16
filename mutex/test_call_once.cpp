/*************************************************************************
	> File Name: test_call_once.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年08月16日 星期四 19时56分43秒
 ************************************************************************/

#include<iostream>
#include<thread>
    // std::this_thread::sleep_for, std::thread
#include<chrono>
    // std::chrono::milliseconds
#include <mutex>
    // std::call_once, std::once_flag

using namespace std;


std::once_flag winner_flag;

int winner;

void set_winner(int x) {
  cout << "x: " << x << "\n";
  winner = x;
}

void wait_1000ms(int id) {
    // count to 1000, waiting 1ms between increments;
    for (int i = 0; i < 1000; ++i) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    // claim to be the winner (only the first such call is executed):
    std::call_once(winner_flag, set_winner, id);
}

int main(int argc, char* argv[]) {
    std::thread threads[10];

    for (int i = 0; i < 10; i++) {
      threads[i] = std::thread(wait_1000ms, i+1);
    }

    cout << "waiting for the first among 10 threads to count 1000ms...\n";

    for (auto& th : threads) {
      th.join();
    }
    cout << "winner thread: " << winner << "\n";

    return 0;
}
