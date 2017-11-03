#include <deque>
#include <stdio.h>



int main(int argc, char *argv[]) {
  printf("**************test case 1****\n");
  std::deque<int> d = {7, 5, 16, 8};

  // Add an integer to the beginning of the deque
  d.push_front(13);
  // Add an integer to the end of the deque
  d.push_back(25);

    printf("[");
    for(int n : d) {
      printf("%d ", n);
    }
    printf("]\n");





    for(std::deque<int>::iterator it = d.begin(); it != d.end(); ++it) {
      printf("%d, ", *it);
    }
    printf("\n");





}
