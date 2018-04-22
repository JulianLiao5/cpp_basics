
#include <iostream>
#include <vector>


int main(int argc, char** argv) {
    std::vector<int> myvec;
    myvec.push_back(3);
    myvec.push_back(2);
    myvec.push_back(1);

    for (auto var : myvec) {
      std::cout << var << std::endl;
    }

    return 0;
}

