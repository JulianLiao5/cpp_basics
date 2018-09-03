/*************************************************************************
	> File Name: test_call_once.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年08月16日 星期四 19时56分43秒
 ************************************************************************/

#include <stdio.h>
#include<iostream>
#include<thread>
// std::thread, std::thread::id, std::thread::get_id

using namespace std;

std::thread::id main_thread_id = std::this_thread::get_id();

void is_main_thread() {
    printf("AA\n");
    if (main_thread_id == std::this_thread::get_id()) {
      cout << "This is the main thread.\n";
    } else {
      cout << "This is NOT the main thread.\n";
    }
}

int main(int argc, char* argv[]) {
    is_main_thread();
    std::thread th(is_main_thread);
    printf("BB\n");
    // th.join();
    printf("CC\n");

    return 0;
}
