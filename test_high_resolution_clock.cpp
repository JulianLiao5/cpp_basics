/*************************************************************************
	> File Name: test_high_resolution_clock.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年10月16日 星期一 10时12分45秒
 ************************************************************************/

#include<iostream>
#include <stdio.h>
#include <chrono>
#include <thread>

using namespace std;



int main(int argc, char *argv[]) {
    using namespace std::chrono;

    std::cout<<"*****************test_case_1****************"<<std::endl;
    high_resolution_clock::time_point now_tp = high_resolution_clock::now();
    long long now_l = static_cast<long long>(duration_cast<std::chrono::milliseconds>(now_tp.time_since_epoch()).count());
    printf("now_d: %lld\n", now_l);

    std::cout<<"*****************test_case_2****************"<<std::endl;

    std::cout<<"*****************test_case_3****************"<<std::endl;

    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    std::cout << "printing out 1000 stars...\n";
    for(int i=0; i< 1000;i++) {
        std::cout<<"+";
    }
    std::cout<<std::endl;

    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

    std::cout<<"It took me "<<time_span.count()<<" seconds."<<std::endl;

    std::cout<<"*****************test_case_4****************"<<std::endl;
    high_resolution_clock::time_point start = high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    high_resolution_clock::time_point end = high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "Waited [" << elapsed.count() << "]ms" << std::endl;
    return 0;
}
