/*************************************************************************
	> File Name: test_condition_variable.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年10月12日 星期四 18时38分31秒
 ************************************************************************/

#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<chrono>
using namespace std;


int value;
std::condition_variable cv;

void read_value() {
    printf("ccccccccccccccccccccccc\n");
    std::cin >> value;
    cv.notify_one();
}

int main(int argc, char *argv[]) {
    std::cout << "Please enter an integer (I'll be printing dots): \n";
    std::thread th(read_value);

    std::mutex mtx;
    std::unique_lock<std::mutex> lck(mtx);

    while(cv.wait_for(lck, std::chrono::seconds(1), [&]{printf("aaaaaaaaaaaa\n");return false;})) {
        printf("bbbbbbbbbbbbbb\n");
        std::cout << "." << std::endl;
    }

    std::cout << "You entered: " << value << '\n';
    th.join();

    std::cout<<"bye.."<<"\n";

    return 0;
}
