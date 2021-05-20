#include <iostream>  // std::cout
#include <algorithm>  // std::sort, std::lower_bound, std::upper_bound
#include <vector>  // std::vector

int main() {
    int myints[] = {4,5,6,6,5,4,5,4,6};
    std::vector<int> v(myints,myints+9);

    std::sort(v.begin(), v.end());

    for(int item : v) {
        std::cout << " " << item;
    }
    std::cout << std::endl;

    std::vector<int>::iterator low,up;
    low = std::lower_bound(v.begin(), v.end(), 5);
    up = std::upper_bound(v.begin(), v.end(), 5);

    std::cout << "lower_bound at position " << (low - v.begin()) << std::endl;
    std::cout << "upper_bound at position " << (up - v.begin()) << std::endl;

    return 0;
}
