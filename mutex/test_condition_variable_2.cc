#include <mutex>
#include <condition_variable>  // std::condition_variable
#include <iostream>
#include <stdio.h>
#include <thread>

std::mutex mtx;
std::condition_variable produce,consume;

int cargo = 0;    // shared value by producers and consumers

void consumer() {
  std::unique_lock<std::mutex> lck(mtx);
  std::thread::id consumer_id = std::this_thread::get_id();
  std::cout << "con thread_id: " << consumer_id << ", cargo: " << cargo << std::endl;
  while (cargo == 0) {
    consume.wait(lck);
  }
  std::cout << cargo << "\n";
  cargo = 0;
  produce.notify_one();
}

void producer(int id) {
  std::unique_lock<std::mutex> lck(mtx);
  std::thread::id producer_id = std::this_thread::get_id();
  std::cout << "pro thread_id: " << producer_id  << ", cargo: " << cargo << std::endl;
  while (cargo != 0) {
    produce.wait(lck);
  }
  cargo = id;
    printf("cargo: %d\n", cargo);
  consume.notify_one();
}

int main(int argc, char* argv[]) {
    std::thread consumers[10],producers[10];
    // spawn 10 consumers and 10 producers
    for (int i = 0; i < 10; i++) {
        printf("Ii: %d\n", i);
        consumers[i] = std::thread(consumer);
        producers[i] = std::thread(producer, i+1);
    }

    // join them back
    for (int i = 0; i < 10; i++) {
        printf("aa - i: %d\n", i);
        producers[i].join();
        printf("bb - i: %d\n", i);
        consumers[i].join();
    }
    return 0;
}
