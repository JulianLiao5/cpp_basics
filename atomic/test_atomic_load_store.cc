#include <iostream>
#include <atomic>
#include <thread>

std::atomic<int> foo(0);


void set_foo(int x) {
    printf("waga!\n");
    foo.store(x, std::memory_order_relaxed);
    printf("haha!\n");
}

void print_foo() {
    printf("1\n");
    int x;
    do {
      printf("2\n");
      x = foo.load(std::memory_order_relaxed);
    } while (x == 0);
    printf("3\n");
    std::cout << "foo: " << x << "\n";
}

int main(int argc, char *argv[]) {
    std::thread first(print_foo);
    printf("stephen!\n");
    std::thread second(set_foo, 10);
    printf("harden!\n");

    first.join();
    printf("a\n");
    second.join();
    printf("b\n");
    return 0;
}
