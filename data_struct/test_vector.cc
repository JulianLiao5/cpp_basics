
#include <climits>
#include <iostream>
#include <vector>


struct PntIdx {
    size_t x_idx;
    size_t y_idx;

    PntIdx(): x_idx(0), y_idx(0) {}
    PntIdx(const size_t x_idx, const size_t y_idx): x_idx(x_idx), y_idx(y_idx) {}
};

int main(int argc, char** argv) {
    unsigned int a;
    printf("sizeof(unsigned int)    :    %lu\n", sizeof(unsigned int));
    printf("sizeof(unsigned int a)    :    %lu\n", sizeof(a));

    printf("UINT_MAX    :    %u\n", UINT_MAX);
    a = UINT_MAX - 1;
    printf("11 - a    :    %u\n", a);
    a += 1;
    printf("22 - a    :    %u\n", a);
    a += 1;
    printf("33 - a    :    %u\n", a);



    std::vector<PntIdx> linePtIdx;
    linePtIdx.push_back(PntIdx(714, 810));
    linePtIdx.push_back(PntIdx(714, 811));
    linePtIdx.push_back(PntIdx(713, 812));
    linePtIdx.push_back(PntIdx(712, 813));
    linePtIdx.push_back(PntIdx(712, 814));
    linePtIdx.push_back(PntIdx(711, 815));
    linePtIdx.push_back(PntIdx(710, 816));
    linePtIdx.push_back(PntIdx(710, 817));
    linePtIdx.push_back(PntIdx(709, 818));
    linePtIdx.push_back(PntIdx(708, 819));
    linePtIdx.push_back(PntIdx(708, 820));
    linePtIdx.push_back(PntIdx(707, 821));
    linePtIdx.push_back(PntIdx(706, 822));
    linePtIdx.push_back(PntIdx(705, 823));

    int size_a = linePtIdx.end() - linePtIdx.begin();
    std::cout << "size_a: " << size_a << ",  size: " << linePtIdx.size() << std::endl;

    return 0;
}

