#include <deque>
#include <stdio.h>
#include <string>


int main(int argc, char *argv[]) {
  int aa;
  unsigned int bb;
  short cc;
  printf("int_size: %ld, unsigned int size: %ld, short size: %ld\n", sizeof(aa), sizeof(bb), sizeof(cc));

  std::string s1 = "0051";
  std::string s2 =  "00 01 00 00 00 00 00 00 ";
  printf("s1.size: %ld, s2.size: %ld\n", s1.size(), s2.size());

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
