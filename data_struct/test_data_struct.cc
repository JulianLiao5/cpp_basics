#include <deque>
#include <stdio.h>
#include <string>

#include <iostream>
#include <limits.h>

int main(int argc, char *argv[]) {

  /**------------------BEGIN char-----------------------**/
  /**
  char cc2;
  signed char sc1;
  unsigned char usc1;
  printf("line: %d, char_size: %ld, signed_char_size: %ld, unsigned_char_size: %ld\n", __LINE__, sizeof(cc2), sizeof(sc1), sizeof(usc1));
  std::cout << __LINE__ << "  min_signed_char: " << SCHAR_MIN << ", max_signed_char: " << SCHAR_MAX << std::endl;
  std::cout << __LINE__ << "  min_char: " << CHAR_MIN << ", max_char: " << CHAR_MAX << std::endl;
  std::cout << __LINE__ << "  max_unsigned_char: " << UCHAR_MAX << std::endl;
  **/
  /**------------------END char-----------------------**/
  int aa;
  unsigned int bb;
  short int cc;
  short dd;
  uint8_t a1;
  int8_t a2;
  uint16_t b1;
  int16_t b2;
  uint32_t c1;
  int32_t c2;
  uint64_t d1;
  int64_t d2;
  printf("int_size: %ld, unsigned int size: %ld, short_int_size: %ld, short size: %ld\n", sizeof(aa), sizeof(bb), sizeof(cc), sizeof(dd));

  printf("uint8_t_size: %ld, int8_t size: %ld, uint16_t_size: %ld, int16_t size: %ld\n", sizeof(a1), sizeof(a2), sizeof(b1), sizeof(b2));
  printf("uint32_t_size: %ld, int32_t size: %ld, uint64_t_size: %ld, int64_t size: %ld\n", sizeof(c1), sizeof(c2), sizeof(d1), sizeof(d2));

/**
  printf("min_int8_t: %d, max_int8_t: %d\n", MIN_OF(int8_t), MAX_OF(int8_t));
  printf("min_int16_t: %d, max_int16_t: %d\n", MIN_OF(int16_t), MAX_OF(int16_t));
  printf("min_int32_t: %d, max_int32_t: %d\n", MIN_OF(int32_t), MAX_OF(int32_t));
**/

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
