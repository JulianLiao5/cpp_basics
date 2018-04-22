
#include <iostream>
#include <vector>


int main(int argc, char **argv) {
    std::vector<int> myvector(3, 100);
    std::vector<int>::iterator it;
        // 100, 100, 100

    it = myvector.begin();
    it = myvector.insert(it, 200);
        // 200, 100, 100, 100

    myvector.insert(it, 2, 300);
        // 300, 300, 200, 100, 100, 100

    // "it" no longer valid, get a new one:
    it = myvector.begin();

    std::vector<int> anothervector(2, 400);
    myvector.insert(it + 2, anothervector.begin(), anothervector.end());
        // 300, 300, 400, 400, 200, 100, 100, 100

    int myarray[] = {501, 502, 503};
    myvector.insert(myvector.begin(), myarray, myarray + 2);

    std::cout << "myvector contains:";
    for (it = myvector.begin(); it < myvector.end(); it++) {
        std::cout << " " << *it;
    }
    std::cout << "\n";

    return 0;
}
